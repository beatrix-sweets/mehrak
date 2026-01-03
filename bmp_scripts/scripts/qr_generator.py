# /// script
# requires-python = ">=3.13"
# dependencies = [
#     "qrcode[pil]>=8.2",
# ]
# ///

#  uv run qr_generator.py
import qrcode
from PIL import Image

# Create QR code instance with Version 4
qr = qrcode.QRCode(
    version=4,  # Version 4 = 33x33
    error_correction=qrcode.constants.ERROR_CORRECT_L,
    box_size=1,
    border=4,
)

# Add your URL
account_name = "hilary.sweets"
url = f"https://www.instagram.com/{account_name}"
qr.add_data(url)
qr.make(fit=False)

# Create and save the image
img = qr.make_image(fill_color="black", back_color="white")
img_rgb = img.convert("RGB")

# Get current size (should be 41x41)
qr_width, qr_height = img_rgb.size
print(f"QR code size: {qr_width}x{qr_height}")

# Create a 64x64 black canvas
TARGET_SIZE = 64
canvas = Image.new("RGB", (TARGET_SIZE, TARGET_SIZE), color=(0, 0, 0))

# Calculate position to center the QR code
x_offset = (TARGET_SIZE - qr_width) // 2
y_offset = (TARGET_SIZE - qr_height) // 2

# Paste the QR code onto the black canvas
canvas.paste(img_rgb, (x_offset, y_offset))

out_file = "../images/bmp/instagram_qr.bmp"
canvas.save(out_file, format="BMP")
print(f"QR code saved as {out_file} (64x64 with QR centered)")