#!/usr/bin/env python

from solcx import compile_standard, install_solc
import json
import os
from web3 import Web3
import eth_keys
from eth_account import account
from dotenv import load_dotenv

def get_private_key(password='My_pass', keystore_file='a.json'):
    keyfile = open('./' + keystore_file)
    keyfile_contents = keyfile.read()
    keyfile.close()
    private_key = eth_keys.keys.PrivateKey(account.Account.decrypt(keyfile_contents, password))
    private_key_str = str(private_key)
    return private_key_str


load_dotenv()

with open("./zen1155_oz501.sol", "r") as file:
    simple_storage_file = file.read()

    install_solc("0.8.20")

    compiled_sol = compile_standard(
        {
            "language": "Solidity",
            "sources": {"zen1155_oz501.sol": {"content": simple_storage_file}},
            "settings": {
                "outputSelection": {
                    "*": {"*": ["abi", "metadata", "evm.bytecode", "evm.sourceMap"]}
                },
                "evmVersion": "paris"
            },
        },
        solc_binary="/usr/local/lib/python3.9/site-packages/solcx/bin/solc-v0.8.20",
        allow_paths="/root/5.0.1/"
    )

with open("compiled_code.json", "w") as file:
    json.dump(compiled_sol, file)


# get bytecode
bytecode = compiled_sol["contracts"]["zen1155_oz501.sol"]["zen1155"]["evm"]["bytecode"]["object"]

# get abi
abi = json.loads(compiled_sol["contracts"]["zen1155_oz501.sol"]["zen1155"]["metadata"])["output"]["abi"]

print(abi)



# For connecting to geth_rpc
w3 = Web3(Web3.HTTPProvider(os.getenv("GETH_RPC_URL")))

#Check Connection
t=w3.is_connected()
print(f"Connected {t}")

chain_id = int(os.getenv("NETWORK_ID"))
private_key = get_private_key()

# Create a signer wallet
PA=w3.eth.account.from_key(private_key)

# Get public address from a signer wallet
my_address = PA.address
print(f"Address {my_address}")
checksum_address = w3.to_checksum_address(my_address)

#Print balance on current accaunt
BA=w3.eth.get_balance(my_address)
print(f"Balance {BA}")

# Create the contract in Python
SimpleStorage = w3.eth.contract(abi=abi, bytecode=bytecode)

# Get the latest transaction
nonce = w3.eth.get_transaction_count(my_address)

# Submit the transaction that deploys the contract
transaction = SimpleStorage.constructor().build_transaction(
    {
        "chainId": chain_id,
        "gasPrice": w3.eth.gas_price,
        "from": my_address,
        "nonce": nonce,
#        'gas': 8000000,
    }
)

# Sign the transaction
signed_txn = w3.eth.account.sign_transaction(transaction, private_key=private_key)
print("Deploying Contract!")

# Send it!
tx_hash = w3.eth.send_raw_transaction(signed_txn.rawTransaction)
# Wait for the transaction to be mined, and get the transaction receipt
print("Waiting for transaction to finish...")
tx_receipt = w3.eth.wait_for_transaction_receipt(tx_hash)
print(f"Done! Contract deployed to {tx_receipt.contractAddress}")

#Verification that code realy deployed (With OpenZeppilin 3.1.0 all work for ERC 1155)
my_code = w3.eth.get_code(tx_receipt.contractAddress)
print(f"Verify code after deployed (Must be NOT b\' \' or change gas value to maximum 8000000) {my_code}")