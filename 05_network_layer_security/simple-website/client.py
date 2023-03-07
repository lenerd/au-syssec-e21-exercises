import sys
import requests
from Crypto.Cipher import PKCS1_OAEP
from Crypto.PublicKey import RSA

username = 'dfaranha'
password = 'OMGbutterflies'
secret = b'The abyss is looking back at you'


def login(session, base_url):
    res = session.post(f'{base_url}/login/', data={'username': username, 'password': password})
    print(res)


def upload(session, base_url):
    res = session.get(f'{base_url}/pk/')
    pk = RSA.import_key(res.text)
    cipher = PKCS1_OAEP.new(pk)
    ciphertext = cipher.encrypt(secret)
    res = session.post(f'{base_url}/upload_secrets/', data={'ciphertext': ciphertext.hex()})
    print(res)


def main(host, port):
    base_url = f'http://{host}:{port}'
    session = requests.session()
    login(session, base_url)
    upload(session, base_url)


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print(f'usage: {sys.argv[0]} <host> <port>', file=sys.stderr)
        exit(1)
    main(sys.argv[1], sys.argv[2])
