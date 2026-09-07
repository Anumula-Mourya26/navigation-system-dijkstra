import requests

OSRM_URL = "https://router.project-osrm.org/route/v1/driving/"

LOCATIONS = {
    1: {"name": "VNR College", "lat": 17.5849, "lon": 78.3899},
    2: {"name": "Bachupally Cross Road", "lat": 17.5554, "lon": 78.3647},
    3: {"name": "Miyapur Cross Road", "lat": 17.4965, "lon": 78.3560},
    4: {"name": "GSM Mall", "lat": 17.4947, "lon": 78.3397},
    5: {"name": "Gandimaisamma Circle", "lat": 17.6215, "lon": 78.4372},
    6: {"name": "JNTU College", "lat": 17.4936, "lon": 78.3916}
}

BASE_EDGES = [
    (1, 2),
    (2, 3),
    (3, 4),
    (3, 6),
    (4, 5),
    (6, 5)
]

EDGES = []

for source, destination in BASE_EDGES:
    EDGES.append((source, destination))
    EDGES.append((destination, source))


def get_route_data(source_id, destination_id):
    source = LOCATIONS[source_id]
    destination = LOCATIONS[destination_id]

    url = (
        f"{OSRM_URL}"
        f"{source['lon']},{source['lat']};"
        f"{destination['lon']},{destination['lat']}"
        f"?steps=true&overview=false"
    )

    try:
        print(
            f"Fetching "
            f"{source['name']} -> "
            f"{destination['name']}"
        )

        response = requests.get(url, timeout=20)
        data = response.json()

        routes = data.get("routes", [])

        if len(routes) == 0:
            return None

        route = routes[0]

        distance_km = round(
            route["distance"] / 1000,
            2
        )

        duration_min = round(
            route["duration"] / 60
        )

        steps_output = []

        for leg in route["legs"]:

            for step in leg["steps"]:

                maneuver = step.get(
                    "maneuver",
                    {}
                )

                modifier = maneuver.get(
                    "modifier",
                    ""
                )

                maneuver_type = maneuver.get(
                    "type",
                    ""
                )

                turn = "straight"

                if modifier in [
                    "left",
                    "slight left",
                    "sharp left"
                ]:
                    turn = "left"

                elif modifier in [
                    "right",
                    "slight right",
                    "sharp right"
                ]:
                    turn = "right"

                elif maneuver_type == "roundabout":
                    turn = "roundabout"

                elif maneuver_type == "arrive":
                    turn = "arrive"

                road_name = step.get(
                    "name",
                    ""
                ).strip()

                if road_name == "":
                    road_name = "Unnamed_Road"

                step_distance = round(
                    step.get(
                        "distance",
                        0
                    )
                )

                steps_output.append(
                    (
                        turn,
                        step_distance,
                        road_name.replace(
                            " ",
                            "_"
                        )
                    )
                )

        return {
            "distance": distance_km,
            "duration": duration_min,
            "steps": steps_output
        }

    except Exception as e:
        print("Error:", e)
        return None


def generate_graph_file():
    with open("graph.txt", "w") as fp:

        for source, destination in EDGES:

            route = get_route_data(
                source,
                destination
            )

            if route is None:
                continue

            fp.write(
                f"{source} "
                f"{destination} "
                f"{route['distance']} "
                f"{route['duration']}\n"
            )


def generate_routes_file():
    with open("routes.txt", "w") as fp:

        for source, destination in EDGES:

            route = get_route_data(
                source,
                destination
            )

            if route is None:
                continue

            fp.write(
                f"{source} "
                f"{destination}\n"
            )

            for turn, dist, road in route["steps"]:

                fp.write(
                    f"{turn} "
                    f"{dist} "
                    f"{road}\n"
                )

            fp.write("END\n")


def main():
    print(
        "Updating route database...\n"
    )

    generate_graph_file()
    generate_routes_file()

    print("\nDone!")
    print("graph.txt updated")
    print("routes.txt updated")


if __name__ == "__main__":
    main()