# Navigation System Using Dijkstra Algorithm

A graph-based navigation application that calculates the shortest route between selected locations using **Dijkstra's Algorithm**.

The project combines:

- **C** for graph representation and shortest-path computation
- **Adjacency lists** for storing the navigation graph
- **Python** for extracting route data from the OSRM routing service
- **Flask** for connecting the frontend and C backend
- **HTML, CSS, and JavaScript** for the web interface
- Text files for communication and data storage

## Features


- Select a source and destination from predefined Hyderabad locations
- Calculate the shortest route using Dijkstra's Algorithm
- Display route path, distance, and estimated travel time
- Generate navigation directions
- Maintain navigation history
- Load graph and route data dynamically from files

## Project Workflow

1. The user selects a source and destination in the web interface.
2. Flask writes the selected locations to `request.txt`.
3. The C backend reads `request.txt`.
4. The C program loads the graph from `graph.txt`.
5. Dijkstra's Algorithm calculates the shortest path.
6. The result is written to `result.txt`.
7. Flask reads the result and displays it on the webpage.

## Expected Project Structure

Adjust the filenames below if your VS Code project uses different names.

```text
navigation-system/
├── app.py
├── extract_routes.py
├── main.c
├── navigation.h
├── graph.h
├── history.h
├── graph.txt
├── routes.txt
├── request.txt
├── result.txt
├── requirements.txt
├── README.md
└── static/
    ├── style.css
    └── script.js
```

If your HTML file is not embedded in Flask templates, place it in the appropriate `templates/` folder:

```text
templates/
└── index.html
```

## Requirements

- Python 3.x
- Flask
- Requests
- A C compiler such as GCC
- Git
- A web browser
- Internet connection when generating route data through OSRM

## Installation

Create and activate a virtual environment:

### Windows

```bash
python -m venv venv
venv\Scripts\activate
```

Install the Python dependencies:

```bash
pip install -r requirements.txt
```

## Generate Route Data

Run the route extraction script before starting the application:

```bash
python extract_routes.py
```

This generates or updates:

- `graph.txt`
- `routes.txt`

The script uses the OSRM routing service to obtain distance, travel time, and navigation-step information.

## Compile the C Backend

Use GCC to compile the C source files. For example:

```bash
gcc main.c -o navigation.exe
```

If your project contains multiple C source files, compile them together:

```bash
gcc main.c navigation.c graph.c history.c -o navigation.exe
```

Use the actual filenames present in your project.

## Run the Application

Start the Flask server:

```bash
python app.py
```

Open the local address shown in the terminal, usually:

```text
http://127.0.0.1:5000
```

## Important Notes

- Run commands from the project root directory.
- Make sure `graph.txt`, `routes.txt`, `request.txt`, and `result.txt` are located where the C program and Flask application expect them.
- Do not commit the Python virtual environment folder.
- Do not commit API keys, passwords, or other private credentials.
- The project currently uses predefined locations and route connections.
- Route data depends on the external OSRM routing service.

## Data Structures and Algorithms Used

- Weighted graph
- Adjacency list
- Linked list
- Dijkstra's shortest-path algorithm
- File handling
- Frontend-backend communication

## Future Enhancements

- Larger road networks
- Real-time traffic information
- GPS integration
- Dynamic route updates
- More locations and user-defined destinations

## Academic Project

This project was developed as a Course Based Project for the Data Structures Laboratory course.

**Project title:** Navigation System Using Dijkstra Algorithm
