# Derleme
Hem server hem clienti derlemek icin:
```
make all
```
Sadece server:
```
make server
```
Sadece client:
```
make client
```
Clean:
```
make clean
```
# Kullanım
Server:
```
./UKSU_server -p 6666 -p password -u username
```
Client:
```
./UKSU_client -p 6666 -p password -u username -h localhost
```
##### Not: Argumanlar verilirken port numarası password den önce verilmelidir.
# Bilgiler
Server sadece bir cliente hizmet vermektedir. Bir client bağlandıktan sonra bağlanan clientlar server tarafından gönderilen bir mesaj ile karşılanıp, kapatılmaktadır. 
Server Ctrl + C(SIGINT) ile kapanmaktadır. 
Client eğer bir komut çalıştırmışsa SIGINT o komutu sonlandırır, eğer herhangi bir komut çalışmıyorken bu sinyali alırsa client kapanmaktadır. 
Komut uzunluğu maximum 1024 karakter olarak tanımlanmıştır.
Veri aktarımı maximum 1024 karakter büyüklüğünde bufferlar ile yapılmaktadır.
sudo kullanılamamalıdır. Kullanıldığı taktirde server tarafı şifre bekleme kısmında takılır ve client serverdan komut çıktısı bekleme kısmında takılır ve bu durum deadlock a neden olur.