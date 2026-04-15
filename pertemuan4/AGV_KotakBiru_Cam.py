# Script python sebagai pengontrol RC lewat deteksi kotak biru via CCTV
# by : nyoman yudi kurniawan
# untuk : TRSE Undiksha 2026
# www.aisi555.com

import cv2
import numpy as np
import socket
import time

# --- KONFIGURASI NETWORK UDP - Sesuaikan ip wemos kamu---
ESP_IP = "192.168.0.10"
ESP_PORT = 7788
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


def send_command(cmd):
    sock.sendto(cmd.encode(), (ESP_IP, ESP_PORT))
    print(f"UDP Sent: {cmd} at {time.strftime('%H:%M:%S')}")


# --- KONFIGURASI VIDEO & WARNA ---
#default memakai resolusi SD, sesuaikan dengan kemampuan PC kamu
rtsp_url = "rtsp://admin:FJUHGV@192.168.0.200:554/Streaming/Channels/102"
#jika spek PC kamu cukup cepat bisa menggunakan versi HD agar deteksinya lebih jauh
#rtsp_url = "rtsp://admin:FJUHGV@192.168.0.200:554/Streaming/Channels/101"
cap = cv2.VideoCapture(rtsp_url)

lower_blue = np.array([90, 100, 50])
upper_blue = np.array([130, 255, 255])

# --- VARIABEL LOGIKA WAKTU ---
last_send_time = 0
send_interval = 0.8  # 800 ms

object_lost_time = None
backup_count = 0  # Menghitung berapa kali sudah mundur (max 2)
last_backup_time = 0

while True:
    ret, frame = cap.read()
    if not ret: break

    h_frame, w_frame, _ = frame.shape
    sepertiga = w_frame // 3
    dua_pertiga = (w_frame // 3) * 2

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, lower_blue, upper_blue)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, np.ones((5, 5), np.uint8))

    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    found_now = False
    current_pos = "0"
    current_arah = "berhenti"
    current_time = time.time()

    for cnt in contours:
        if cv2.contourArea(cnt) > 2000:
            peri = cv2.arcLength(cnt, True)
            approx = cv2.approxPolyDP(cnt, 0.04 * peri, True)

            if len(approx) == 4:
                found_now = True
                # Reset status pencarian jika objek ketemu
                object_lost_time = None
                backup_count = 0

                x, y, w, h = cv2.boundingRect(approx)
                center_x = x + (w // 2)

                if center_x < sepertiga:
                    current_pos = "3"  # Kiri
                    current_arah = "kiri"
                elif center_x > dua_pertiga:
                    current_pos = "4"  # Kanan
                    current_arah = "kanan"
                else:
                    current_pos = "1"  # Maju
                    current_arah = "depan"

                # --- LOGIKA 1: KIRIM TIAP 800MS SAAT ADA DETEKSI ---
                if current_time - last_send_time > send_interval:
                    send_command(current_pos)
                    last_send_time = current_time

                cv2.drawContours(frame, [approx], -1, (0, 255, 255), 3)

    # --- LOGIKA 2: MUNDUR SEKALI TIAP 5 DETIK (MAKS 2 KALI) ---
    if not found_now:
        # Jika baru saja hilang, kirim perintah berhenti sekali dan catat waktunya
        if object_lost_time is None:
            send_command("0")
            object_lost_time = current_time
            last_backup_time = current_time
            print("Objek hilang. Menunggu 5 detik...")

        # Cek jika sudah 5 detik sejak terakhir mundur/hilang DAN belum mencapai 2 kali mundur
        elif backup_count < 2:
            if current_time - last_backup_time >= 5.0:
                send_command("2")  # Kirim mundur SEKALI
                backup_count += 1
                last_backup_time = current_time
                print(f"Mundur ke-{backup_count} dilakukan.")

    # Visualisasi status di frame
    status = f"Deteksi di { current_arah} " if found_now else f"Kosong (Mundur: {backup_count}/2)"
    cv2.putText(frame, f"Status: {status}", (110, 35), 0, 0.7, (0, 255, 255), 2)

    # Gambar garis pembagi di layar (Opsional untuk visualisasi)
    cv2.line(frame, (sepertiga, 0), (sepertiga, h_frame), (255, 255, 255), 2)
    cv2.line(frame, (dua_pertiga, 0), (dua_pertiga, h_frame), (255, 255, 255), 2)

    cv2.imshow("Area Of Interest - UDP - AGV ", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        send_command("0")
        break

cap.release()
cv2.destroyAllWindows()
