// app.js

// Step 2.1: Create a WebSocket connection
const ws = new WebSocket(`ws://${window.location.hostname}/ws`);

// Step 2.2: Define what to do when a message is received from the WebSocket
ws.onmessage = (event) => {
    // Parse the JSON data from the WebSocket message
    const data = JSON.parse(event.data);
    console.log("State Update:", data); // Log data for debugging

    // Step 2.3: Update the HTML elements based on the received data
    document.getElementById("led1Status").textContent = data.LED1 ? "ON" : "OFF";
    document.getElementById("led2Status").textContent = data.LED2 ? "ON" : "OFF";
    document.getElementById("switchStatus").textContent = data.Switch ? "ON" : "OFF";
};

// Step 2.4: Define what happens when the WebSocket connection is opened
ws.onopen = () => {
    console.log("Connected to WebSocket server");
};

// Step 2.5: Define what happens when the WebSocket connection is closed
ws.onclose = () => {
    console.log("Disconnected from WebSocket server");
};
