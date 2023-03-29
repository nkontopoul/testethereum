//First, you need to install the Nethereum package using NuGet:

//Install-Package Nethereum.Web3 -Version 4.0.2

//Then, you can use the following code to create Ethereum wallets and check their balances:



using System;
using System.IO;
using System.Threading.Tasks;
using Nethereum.Hex.HexConvertors.Extensions;
using Nethereum.Util;
using Nethereum.Web3;
using Nethereum.Web3.Accounts;

namespace EthereumWalletGenerator
{
    class Program
    {
        private static readonly string InfuraProjectId = "YOUR-INFURA-PROJECT-ID";
        private static readonly string InfuraUrl = $"https://mainnet.infura.io/v3/{InfuraProjectId}";

        static async Task Main(string[] args)
        {
            int numWallets = 1000;
            await CreateWalletsAndCheckBalances(numWallets);
        }

        private static async Task CreateWalletsAndCheckBalances(int numWallets)
        {
            var web3 = new Web3(InfuraUrl);

            for (int i = 0; i < numWallets; i++)
            {
                var account = EthECKey.GenerateKey();
                string address = account.GetPublicAddress();
                string privateKey = account.GetPrivateKeyAsBytes().ToHex(true);

                var balance = await CheckBalance(web3, address);

                if (balance > 0)
                {
                    Console.WriteLine($"Found address with balance: {address}");
                    SaveToFile(address, privateKey);
                }
            }
        }

        private static async Task<decimal> CheckBalance(Web3 web3, string address)
        {
            var balanceWei = await web3.Eth.GetBalance.SendRequestAsync(address);
            var weiToEthConverter = new UnitConversion();
            return weiToEthConverter.FromWeiToBigDecimal(balanceWei.Value);
        }

        private static void SaveToFile(string address, string privateKey)
        {
            using (var streamWriter = new StreamWriter("found.txt", true))
            {
                streamWriter.WriteLine($"Address: {address}");
                streamWriter.WriteLine($"Private Key: {privateKey}");
                streamWriter.WriteLine();
            }
        }
    }
}

//Remember to replace YOUR-INFURA-PROJECT-ID with your own Infura project ID. You can obtain one by signing up on the Infura website (https://infura.io/).

//Please use this script responsibly and do not use it for malicious purposes.
