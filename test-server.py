from flask import Flask, request, jsonify, render_template

app = Flask(__name__)

# Simpan data yang diterima dalam list
data_store = []

@app.route('/api/data', methods=['POST'])
def receive_data():
    try:
        # Mengambil data JSON dari permintaan
        data = request.get_json()

        # Cetak data yang diterima ke konsol (atau lakukan pemrosesan lain)
        print(f"Data received: {data}")

        # Menyimpan data ke data_store
        data_store.append(data)

        # Mengirim respon balik ke klien
        response = {
            'status': 'success',
            'data_received': data
        }
        return jsonify(response), 200

    except Exception as e:
        # Tangani kesalahan
        print(f"Error: {e}")
        response = {
            'status': 'error',
            'message': str(e)
        }
        return jsonify(response), 400

@app.route('/api/data', methods=['GET'])
def get_data():
    try:
        # Mengirim data yang disimpan sebagai respon
        response = {
            'status': 'success',
            'data': data_store
        }
        return jsonify(response), 200

    except Exception as e:
        # Tangani kesalahan
        print(f"Error: {e}")
        response = {
            'status': 'error',
            'message': str(e)
        }
        return jsonify(response), 400


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
