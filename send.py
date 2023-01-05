import ftplib


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


hostname = "192.168.0.0"

upload_src_path = "./yw2-0.7.4.3gx"
upload_dst_path = "STOR /luma/plugins/0004000000155100/yw2-0.7.4.3gx"

port = 5000
timeout = 500

ftp_upload(hostname, port, upload_src_path, upload_dst_path, timeout)
