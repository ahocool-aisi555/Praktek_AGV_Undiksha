# Script python untuk deteksi kotak warna biru
# by : nyoman yudi kurniawan
# untuk : TRSE Undiksha 2026
# www.aisi555.com

import cv2
import numpy as np

# Konfigurasi RTSP
rtsp_url = "rtsp://admin:FJUHGV@192.168.0.200:554/Streaming/Channels/102"
cap = cv2.VideoCapture(rtsp_url)

# --- RENTANG WARNA BIRU (HSV) ---
lower_blue = np.array([90, 100, 50])
upper_blue = np.array([130, 255, 255])

while True:
    ret, frame = cap.read()
    if not ret:
        print("Gagal menerima stream...")
        break

    # 1. Konversi ke HSV untuk masking warna
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, lower_blue, upper_blue)

    # 2. Pembersihan noise agar deteksi lebih solid
    kernel = np.ones((5, 5), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)

    # 3. Cari kontur pada area yang berwarna biru
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    for cnt in contours:
        # Filter berdasarkan luas objek agar noise kecil hilang
        if cv2.contourArea(cnt) > 1500:

            # Pendekatan poligon untuk mengecek jumlah sudut
            peri = cv2.arcLength(cnt, True)
            approx = cv2.approxPolyDP(cnt, 0.04 * peri, True)

            # Jika sudutnya 4, kita anggap itu kotak
            if len(approx) == 4:
                x, y, w, h = cv2.boundingRect(approx)

                # --- VISUALISASI ---
                # Warna Kuning dalam BGR adalah (0, 255, 255)
                warna_kuning = (0, 255, 255)

                # Gambar kotak kuning (Rectangle)
                cv2.drawContours(frame, [approx], -1, warna_kuning, 3)

                # Tambahkan teks "KOTAK BIRU" di atas kotak
                cv2.putText(frame, "KOTAK BIRU", (x, y - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.8, warna_kuning, 2)

    # 4. Tampilkan hasil akhir
    cv2.imshow("Deteksi Kotak Biru - Penanda Kuning", frame)

    # Keluar dengan menekan tombol 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
