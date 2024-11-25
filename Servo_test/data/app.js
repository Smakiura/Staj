let socket = new WebSocket("ws://10.10.27.6:80/ws");  // Ensure IP is correct for ESP32
let scanLines = [];  // Store scan lines with timestamp for f10.10.27.6:80ading

// WebSocket events
socket.onopen = () => console.log("Connected to WebSocket");
socket.onmessage = (event) => {
    let data = event.data.split(';');
    data.forEach(item => {
        let [angle, distance] = item.split(',');
        updateRadar(parseInt(angle), parseInt(distance));
    });
};
socket.onerror = (error) => console.error("WebSocket error: ", error);
socket.onclose = () => console.log("WebSocket closed");

const canvas = document.getElementById("radarCanvas");
const ctx = canvas.getContext("2d");

// Function to update the radar based on received angle and distance
function updateRadar(angle, distance) {
    let lineRadian = (- angle) * Math.PI / 180;  // Adjust angle for 180-degree scan

    // Clear the canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // Draw distance lines
    drawDistanceLines();

    // Add the current scan line (based on angle and distance)
    let lineLength = Math.min(distance * 1.5, 290);  // Ensure max length of 290
    scanLines.push({ angle, distance, time: Date.now() });

    // Draw the scan lines with fading
    scanLines.forEach((line, index) => {
        let lineRadian = (- line.angle) * Math.PI / 180;  // Adjust angle for 180-degree scan
        let lineLength = Math.min(line.distance * 1.5, 290);

        // Calculate fading based on time elapsed
        let fadeTime = Date.now() - line.time;
        let alpha = Math.max(0, Math.min(1, 1 - fadeTime / 100));  // Fade over 1 second

        ctx.beginPath();
        ctx.moveTo(300, 300);
        ctx.lineTo(300 + lineLength * Math.cos(lineRadian), 300 + lineLength * Math.sin(lineRadian));
        ctx.strokeStyle = `rgba(255, 0, 0, ${alpha})`;  // Red line with fade effect
        ctx.lineWidth = 4;
        ctx.stroke();

       
    });
}

// Function to draw the distance lines (1m, 2m, 3m)
function drawDistanceLines() {
    const distances = [1, 2, 3];
    distances.forEach((dist) => {
        ctx.beginPath();
        ctx.arc(300, 300, dist * 100, Math.PI, 0, false);  // Draw concentric arcs for each distance
        ctx.strokeStyle = '#00ff00';  // Green arc for the radar
        ctx.lineWidth = 1;
        ctx.stroke();
        ctx.setLineDash([]);  // Reset dashed lines

        // Label distance lines
        ctx.fillStyle = '#ffffff';
        ctx.font = "18px Arial";
        ctx.fillText(`${dist}m`, 300 + (dist * 100) * Math.cos(Math.PI), 300 + (dist * 100) * Math.sin(Math.PI));
    });
}
