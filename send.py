import ftplib
import os
from dotenv import load_dotenv

def ftp_upload(hostname, port, upload_src_path, upload_dst_path, timeout):
    # FTP接続/アップロード
    with ftplib.FTP() as ftp:
        try:
            ftp.connect(host=hostname, port=port, timeout=timeout)
            # パッシブモード設定
            ftp.set_pasv("true")

            with open(upload_src_path, "rb") as fp:
                ftp.storbinary(upload_dst_path, fp)

        except ftplib.all_errors as e:
            pass

load_dotenv()

hostname = os.getenv("HOSTNAME")

upload_src_path = "./ideaCTRPF.3gx"
upload_dst_path = "STOR /luma/plugins/" + os.getenv("TITLE_ID") + "/ideaCTRPF.3gx"

port = 5000
timeout = 500

ftp_upload(hostname, port, upload_src_path, upload_dst_path, timeout)
