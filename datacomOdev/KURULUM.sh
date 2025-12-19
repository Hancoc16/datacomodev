#!/bin/bash
# Kurulum Scripti - İlk defa çalıştırırken kullanın

echo "=== Gerekli Araçları Kuruyor ==="
echo ""

# Güncelle
echo "Paket listesi güncelleniyor..."
sudo apt update

# Build araçlarını kur
echo "Build araçları kuruluyor (g++, make)..."
sudo apt install -y build-essential

echo ""
echo "✓ Kurulum tamamlandı!"
echo ""
echo "Şimdi projeyi derlemek için:"
echo "  cd /mnt/c/Users/hexShade/Desktop/datacom"
echo "  make"

