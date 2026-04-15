import cv2
import numpy as np

# Konfigurasi RTSP
rtsp_url = "rtsp://admin:FJUHGV@192.168.0.200:554/Streaming/Channels/102"
cap = cv2.VideoCapture(rtsp_url)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # 1. Konversi ke HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # 2. Definisi Rentang Warna Merah (Dua bagian di spektrum HSV)
    # Merah bawah (0-10)
    lower_red1 = np.array([0, 120, 70])
    upper_red1 = np.array([10, 255, 255])
    # Merah atas (170-180)
    lower_red2 = np.array([170, 120, 70])
    upper_red2 = np.array([180, 255, 255])

    # Buat Mask dan Gabungkan
    mask1 = cv2.inRange(hsv, lower_red1, upper_red1)
    mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
    mask = cv2.add(mask1, mask2)

    # 3. Pembersihan Noise (Morfologi)
    kernel = np.ones((5, 5), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)

    # 4. Cari Kontur
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    for cnt in contours:
        # Filter luas minimal
        if cv2.contourArea(cnt) > 1000:

            # Hitung jumlah sudut
            peri = cv2.arcLength(cnt, True)
            approx = cv2.approxPolyDP(cnt, 0.04 * peri, True)

            # Jika sudutnya 3, maka itu Segitiga
            if len(approx) == 3:
                x, y, w, h = cv2.boundingRect(approx)

                # Gunakan penanda warna Kuning (0, 255, 255)
                warna_penanda = (0, 255, 255)

                # Gambar segitiga
                cv2.drawContours(frame, [approx], -1, warna_penanda, 3)

                # Tampilkan teks
                cv2.putText(frame, "SEGITIGA MERAH", (x, y - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.8, warna_penanda, 2)

    # Tampilkan hasil
    cv2.imshow("Deteksi Segitiga Merah", frame)

    # Debug: melihat area merah yang tertangkap mask
    # cv2.imshow("Mask Merah", mask)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()