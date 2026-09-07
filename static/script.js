async function deleteHistory(index) {
    await fetch("/delete_history", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({ index: index })
    });
    
    loadHistory();
}

async function loadHistory() {
    try {
        const response = await fetch("/history");
        const history = await response.json();
        let historyHTML = "";

        if(history.length === 0) {
            historyHTML = "No History Yet";
        } else {
            history.forEach((item, index) => {
                historyHTML += `
                <div class="history-item">
                    <span>${item}</span>
                    <button class="delete-btn" onclick="deleteHistory(${index})">❌</button>
                </div>
                `;
            });
        }

        document.getElementById("historyBox").innerHTML = historyHTML;
    } catch(error) {
        console.log(error);
    }
}

document.getElementById("navigateBtn").addEventListener("click", async () => {
    const sourceSelect = document.getElementById("source");
    const destinationSelect = document.getElementById("destination");

    const source = sourceSelect.value;
    const destination = destinationSelect.value;

    const sourceText = sourceSelect.options[sourceSelect.selectedIndex].text;
    const destinationText = destinationSelect.options[destinationSelect.selectedIndex].text;

    const response = await fetch("/navigate", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({ source, destination, sourceText, destinationText })
    });

    const data = await response.json();
    const result = data.result;

    const pathMatch = result.match(/PATH:\s*([\s\S]*?)DISTANCE:/i);

    if(pathMatch) {
        let pathText = pathMatch[1].trim();
        pathText = pathText.replaceAll("-->", "<br>↓<br>");
        document.getElementById("routePath").innerHTML = pathText;
    }

    const distanceMatch = result.match(/DISTANCE:\s*([\d.]+)/i);
    const timeMatch = result.match(/TIME:\s*(\d+)/i);

    if(distanceMatch) {
        document.getElementById("distance").innerText = distanceMatch[1] + " km";
        document.getElementById("middleDistance").innerText = distanceMatch[1] + " km";
    }

    if(timeMatch) {
        document.getElementById("time").innerText = timeMatch[1] + " min";
        document.getElementById("middleTime").innerText = timeMatch[1] + " min";
    }

    const directionsIndex = result.indexOf("DIRECTIONS:");

    if(directionsIndex !== -1) {
        document.getElementById("directionsBox").innerHTML = "<pre>" + result.substring(directionsIndex) + "</pre>";
    }

    loadHistory();
});

loadHistory();