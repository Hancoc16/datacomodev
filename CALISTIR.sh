#!/bin/bash
# Çalıştırma Scripti - Doğru dizinde çalıştırır

# Proje dizinine git
cd /mnt/c/Users/hexShade/Desktop/datacom 2>/dev/null || {
    echo "Proje dizini bulunamadı: /mnt/c/Users/hexShade/Desktop/datacom"
    echo "Lütfen doğru dizinde olduğunuzdan emin olun."
    exit 1
}

# Hangi programı çalıştıracağız?
if [ "$1" = "client1" ] || [ "$1" = "1" ]; then
    echo "Client 1 başlatılıyor..."
    ./client1
elif [ "$1" = "client2" ] || [ "$1" = "2" ]; then
    echo "Client 2 başlatılıyor..."
    ./client2
elif [ "$1" = "server" ] || [ "$1" = "s" ]; then
    echo "Server başlatılıyor..."
    ./server
else
    echo "Kullanım: $0 [client1|client2|server]"
    echo "  veya: $0 [1|2|s]"
    echo ""
    echo "Örnek:"
    echo "  $0 client2    # Client 2'yi başlatır"
    echo "  $0 server    # Server'ı başlatır"
    echo "  $0 client1   # Client 1'i başlatır"
    exit 1
fi

