# WSL (Windows Subsystem for Linux) Kullanım Kılavuzu

## 1. WSL'i Başlatma

### Yöntem 1: PowerShell/CMD'den
```bash
wsl
```
veya belirli bir dağıtımı başlatmak için:
```bash
wsl -d Ubuntu-22.04
```

### Yöntem 2: Başlat Menüsünden
- Windows tuşuna bas
- "Ubuntu" yaz ve Ubuntu 22.04'ü seç

### Yöntem 3: Terminal'den
```bash
ubuntu2204
```

## 2. Projeyi WSL'e Kopyalama

Windows dosyalarınızı WSL'de erişmek için:

### Yöntem 1: Windows Dosyalarına Erişim
WSL'de Windows dosyalarına şu yoldan erişebilirsiniz:
```bash
cd /mnt/c/Users/hexShade/Desktop/datacom
```

### Yöntem 2: WSL Home Dizinine Kopyalama (Önerilen)
```bash
# WSL'i başlat
wsl

# Home dizinine git
cd ~

# Projeyi kopyala (Windows'tan WSL'e)
cp -r /mnt/c/Users/hexShade/Desktop/datacom ~/datacom

# Proje dizinine git
cd ~/datacom
```

## 3. Gerekli Araçları Kurma

WSL'de g++ ve make'in kurulu olup olmadığını kontrol edin:
```bash
g++ --version
make --version
```

Eğer kurulu değilse:
```bash
sudo apt update
sudo apt install -y build-essential
```

## 4. Projeyi Derleme

```bash
# Proje dizinine git
cd ~/datacom

# Derle
make
```

## 5. Projeyi Çalıştırma

**ÖNEMLİ:** Üç ayrı terminal penceresinde çalıştırmanız gerekiyor!

### Terminal 1 - Client 2 (Alıcı)
```bash
cd ~/datacom
./client2
```

### Terminal 2 - Server
```bash
cd ~/datacom
./server
```

### Terminal 3 - Client 1 (Gönderici)
```bash
cd ~/datacom
./client1
```

## 6. WSL Terminal Pencereleri Açma

### Yöntem 1: Windows Terminal Kullan (Önerilen)
- Windows Terminal'i aç
- Yeni sekme ekle (Ctrl+Shift+T veya + butonu)
- Her sekmede `wsl` yaz

### Yöntem 2: Ubuntu'yu Birden Fazla Kez Başlat
- Başlat menüsünden Ubuntu'yu aç
- Her biri ayrı terminal penceresi olacak

### Yöntem 3: WSL'de tmux Kullan
```bash
# tmux kur (eğer yoksa)
sudo apt install tmux

# tmux başlat
tmux

# Yeni pencere: Ctrl+B sonra C
# Pencere değiştir: Ctrl+B sonra 0, 1, 2
```

## Hızlı Başlangıç Komutları

```bash
# 1. WSL'i başlat ve projeye git
wsl
cd ~/datacom || cd /mnt/c/Users/hexShade/Desktop/datacom

# 2. Derle
make

# 3. Üç ayrı terminalde çalıştır:
# Terminal 1: ./client2
# Terminal 2: ./server  
# Terminal 3: ./client1
```

## Sorun Giderme

### "Permission denied" hatası
```bash
chmod +x client1 client2 server
```

### "Port already in use" hatası
```bash
# Portu kullanan process'i bul
sudo netstat -tulpn | grep 8080
# veya
sudo lsof -i :8080

# Process'i sonlandır
sudo kill -9 <PID>
```

### Derleme hatası
```bash
# Build araçlarını kur
sudo apt update
sudo apt install -y build-essential g++ make
```

## İpuçları

1. **Windows Terminal kullanın** - Birden fazla sekme açmak çok kolay
2. **WSL dosya sistemi daha hızlı** - Dosyaları WSL home dizinine kopyalayın (`~/datacom`)
3. **VS Code WSL extension** - Windows'ta VS Code kullanıyorsanız, WSL extension'ı yükleyin

