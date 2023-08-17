const ctx = document.getElementById('temperatureChart').getContext('2d');
const temperatureChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: [],
    datasets: [
      {
        label: 'Temperature',
        data: [],
        backgroundColor: 'rgba(75, 192, 192, 0.2)',
        borderColor: 'rgba(75, 192, 192, 1)',
        borderWidth: 1,
      },
    ],
  },
  options: {
    // Configure chart options as needed
  },
});

// Fetch and update graph data every few seconds
function fetchDataAndUpdateGraph() {
  fetch('/api/temperature')
    .then(response => response.json())
    .then(data => {
      const timestamp = new Date().toLocaleTimeString();
      const temperature = data.temperature; // Modify this based on your API response

      temperatureChart.data.labels.push(timestamp);
      temperatureChart.data.datasets[0].data.push(temperature);

      temperatureChart.update();
    })
    .catch(error => {
      console.error('Error fetching data:', error);
    });
}

setInterval(fetchDataAndUpdateGraph, 5000); // Fetch data every 5 seconds
