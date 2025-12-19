#!/bin/bash
# Hızlı Başlangıç Scripti

echo "=== WSL Hızlı Başlangıç ==="
echo ""

# Proje dizinine git
cd /mnt/c/Users/hexShade/Desktop/datacom 2>/dev/null || cd ~/datacom 2>/dev/null || {
    echo "Proje dizini bulunamadı!"
    exit 1
}

echo "Proje dizini: $(pwd)"
echo ""

# Gerekli araçları kontrol et
if ! command -v g++ &> /dev/null; then
    echo "g++ bulunamadı. Kuruluyor..."
    sudo apt update
    sudo apt install -y build-essential
fi

if ! command -v make &> /dev/null; then
    echo "make bulunamadı. Kuruluyor..."
    sudo apt install -y build-essential
fi

echo "Derleme başlıyor..."
make

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ Derleme başarılı!"
    echo ""
    echo "Şimdi üç ayrı terminalde çalıştırın:"
    echo "  Terminal 1: ./client2"
    echo "  Terminal 2: ./server"
    echo "  Terminal 3: ./client1"
else
    echo "✗ Derleme başarısız!"
    exit 1
fi

