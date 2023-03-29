#include <iostream>
#include <fstream>
#include <vector>
#include <libdevcrypto/SecretStore.h>
#include <libethcore/KeyManager.h>
#include <libethcore/ICAP.h>
#include <libweb3jsonrpc/AccountHolder.h>
#include <libweb3jsonrpc/JsonHelper.h>
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace std;
using namespace dev;
using namespace dev::eth;

const string INFURA_PROJECT_ID = "YOUR-INFURA-PROJECT-ID";
const string INFURA_URL = "https://mainnet.infura.io/v3/" + INFURA_PROJECT_ID;

bool check_balance(const string& address, double& balance)
{
    jsonrpc::HttpClient httpClient(INFURA_URL);
    dev::rpc::SessionManager sessionManager;
    dev::KeyPair kp = dev::KeyPair::create();
    dev::eth::AccountHolder accountHolder([&] { return kp; }, sessionManager);

    try
    {
        Json::Value request;
        request["jsonrpc"] = "2.0";
        request["id"] = 1;
        request["method"] = "eth_getBalance";
        request["params"] = Json::Value(Json::arrayValue);
        request["params"].append(address);
        request["params"].append("latest");

        Json::Value response = accountHolder.rpcHttpCall(request, httpClient);

        if (response.isMember("result"))
        {
            u256 wei = jsToU256(response["result"].asString());
            balance = dev::eth::units::toEther(wei);
            return true;
        }
    }
    catch (const std::exception& e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    return false;
}

void save_to_file(const string& address, const string& private_key)
{
    ofstream file("found.txt", ios::app);
    if (file.is_open())
    {
        file << "Address: " << address << endl;
        file << "Private Key: " << private_key << endl << endl;
        file.close();
    }
}

int main()
{
    const int num_wallets = 1000;

    for (int i = 0; i < num_wallets; ++i)
    {
        KeyPair key_pair = KeyPair::create();
        string address = to_checksum_address(key_pair.address());
        string private_key = toHex(key_pair.secret().ref());

        double balance = 0;
        if (check_balance(address, balance) && balance > 0)
        {
            cout << "Found address with balance: " << address << endl;
            save_to_file(address, private_key);
        }
    }

    return 0;
}
