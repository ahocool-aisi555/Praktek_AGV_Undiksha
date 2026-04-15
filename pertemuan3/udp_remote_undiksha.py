# Script python sebagai pengontrol RC truck lewat PC 
# by : nyoman yudi kurniawan
# untuk : TRSE Undiksha 2026
# www.aisi555.com


import socket
from pynput import keyboard

# Konfigurasi Network, pastikan sesuai dengan ip esp kamu
ESP_IP = "192.168.0.10"
ESP_PORT = 7788

# Inisialisasi Socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print(f"Mengontrol AGV di {ESP_IP}:{ESP_PORT}")
print("Gunakan Panah untuk Bergerak, Spasi untuk Berhenti, ESC untuk Keluar.")


def send_command(cmd):
    """Mengirim perintah karakter ke ESP8266"""
    sock.sendto(cmd.encode(), (ESP_IP, ESP_PORT))
    # print(f"Sent: {cmd}") # Debugging, uncomment jika diperlukan


def on_press(key):
    try:
        if key == keyboard.Key.up:
            send_command("1")  # Maju
        elif key == keyboard.Key.down:
            send_command("2")  # Mundur
        elif key == keyboard.Key.left:
            send_command("3")  # Kiri
        elif key == keyboard.Key.right:
            send_command("4")  # Kanan
        elif key == keyboard.Key.space:
            send_command("0")  # Berhenti
    except AttributeError:
        pass


def on_release(key):
    # Jika ingin AGV otomatis berhenti saat tombol dilepas,
    # hapus komentar pada baris di bawah ini:
    # send_command("0")

    if key == keyboard.Key.esc:
        # Berhenti mendengarkan keyboard
        return False


# Mulai mendengarkan input keyboard
with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()
