import os
import json
from web3 import Web3
from eth_account import Account
from eth_keys import keys

# Connect to Ethereum node (Infura, for example)
infura_url = 'https://mainnet.infura.io/v3/YOUR-INFURA-PROJECT-ID'
w3 = Web3(Web3.HTTPProvider(infura_url))

# Function to check wallet balance
def check_balance(address):
    balance = w3.eth.getBalance(address)
    return w3.fromWei(balance, 'ether')

# Function to save address and private key if balance is greater than zero
def save_to_file(address, private_key):
    with open('found.txt', 'a') as f:
        f.write(f'Address: {address}\nPrivate Key: {private_key}\n\n')

# Main function to create wallets and check balances
def create_wallets_and_check_balances(num_wallets):
    for _ in range(num_wallets):
        acct = Account.create()
        address = acct.address
        private_key = acct.privateKey.hex()

        balance = check_balance(address)

        if balance > 0:
            print(f'Found address with balance: {address}')
            save_to_file(address, private_key)

if __name__ == '__main__':
    num_wallets = 1000  # Number of wallets to create
    create_wallets_and_check_balances(num_wallets)
