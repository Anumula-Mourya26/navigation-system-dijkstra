from flask import Flask, render_template, request, jsonify
import subprocess
import os

app = Flask(__name__)

HISTORY_FILE = "history.txt"

@app.route("/")
def home():
    return render_template("index.html")

@app.route("/navigate", methods=["POST"])
def navigate():
    data = request.get_json()
    source = data["source"]
    destination = data["destination"]
    source_name = data["sourceText"]
    destination_name = data["destinationText"]

    with open("request.txt", "w") as f:
        f.write(f"{source} {destination}")

    subprocess.run(["navigation.exe"])

    with open("result.txt", "r") as f:
        result = f.read()

    with open(HISTORY_FILE, "a") as f:
        f.write(f"{source_name} -> {destination_name}\n")

    return jsonify({"result": result})

@app.route("/history")
def history():
    if not os.path.exists(HISTORY_FILE):
        return jsonify([])

    with open(HISTORY_FILE, "r") as f:
        lines = [line.strip() for line in f.readlines() if line.strip()]

    lines.reverse()

    return jsonify(lines)

@app.route("/delete_history", methods=["POST"])
def delete_history():
    data = request.get_json()
    index = data["index"]

    if not os.path.exists(HISTORY_FILE):
        return jsonify({"success": False})

    with open(HISTORY_FILE, "r") as f:
        lines = [line.strip() for line in f.readlines() if line.strip()]

    lines.reverse()

    if 0 <= index < len(lines):
        lines.pop(index)

    lines.reverse()

    with open(HISTORY_FILE, "w") as f:
        for line in lines:
            f.write(line + "\n")

    return jsonify({"success": True})

if __name__ == "__main__":
    app.run(debug=True)