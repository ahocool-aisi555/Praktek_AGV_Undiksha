# Dasar deteksi kotak, lingkaran dan segitiga
# by : nyoman yudi kurniawan
# untuk : TRSE Undiksha 2026
# www.aisi555.com

import cv2
import numpy as np

# Konfigurasi RTSP jika menggunakan cctv yang tersedia untuk praktek
rtsp_url = "rtsp://admin:FJUHGV@192.168.0.200:554/Streaming/Channels/102"
cap = cv2.VideoCapture(rtsp_url)
#jika memakai webcam di laptop
# cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # --- PRE-PROCESSING ---
    # Convert ke Gray dan bersihkan noise dengan Median Blur (Sangat penting untuk lingkaran)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray_blurred = cv2.medianBlur(gray, 5)

    # --- 1. DETEKSI LINGKARAN (HoughCircles) ---
    # param1: sensitivitas tepi, param2: ambang deteksi (makin kecil makin banyak lingkaran palsu)
    circles = cv2.HoughCircles(
        gray_blurred,
        cv2.HOUGH_GRADIENT,
        dp=1.2,
        minDist=50,
        param1=100,
        param2=60,
        minRadius=20,
        maxRadius=150
    )

    if circles is not None:
        circles = np.uint16(np.around(circles))
        for i in circles[0, :]:
            # Gambar lingkaran luar
            cv2.circle(frame, (i[0], i[1]), i[2], (0, 255, 0), 3)
            # Gambar pusat
            cv2.circle(frame, (i[0], i[1]), 2, (0, 0, 255), 3)
            cv2.putText(frame, "Lingkaran", (i[0] - 20, i[1] - i[2] - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)

    # --- 2. DETEKSI KOTAK & SEGITIGA (Kontur) ---
    edged = cv2.Canny(gray_blurred, 50, 150)
    contours, _ = cv2.findContours(edged, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    for cnt in contours:
        area = cv2.contourArea(cnt)
        if area < 800: continue  # Filter noise

        peri = cv2.arcLength(cnt, True)
        approx = cv2.approxPolyDP(cnt, 0.04 * peri, True)
        num_corners = len(approx)
        x, y, w, h = cv2.boundingRect(approx)

        # Hanya deteksi jika sudutnya 3 atau 4 (biar tidak bentrok dengan lingkaran)
        if num_corners == 3:
            cv2.drawContours(frame, [approx], -1, (255, 0, 0), 3)
            cv2.putText(frame, "Segitiga", (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)
        elif num_corners == 4:
            cv2.drawContours(frame, [approx], -1, (0, 165, 255), 3)
            cv2.putText(frame, "Kotak", (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 165, 255), 2)

    cv2.imshow("Optimized Shape Detection", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
