# python_client.py - simple client examples using requests (or urllib)
import requests

BASE = "http://127.0.0.1:8080"

def try_endpoint(path):
    url = BASE + path
    r = requests.get(url, timeout=2.0)
    print(path, "->", r.status_code, r.text)

if __name__ == "__main__":
    try_endpoint("/health")
    try_endpoint("/add?a=10&b=32")
    try_endpoint("/sub?a=10&b=32")
    try_endpoint("/mul?a=6&b=7")
    try_endpoint("/div?a=100&b=5")
    try_endpoint("/div?a=10&b=0")
