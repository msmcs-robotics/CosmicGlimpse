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
    scales: {
      xAxes: [{
        type: 'time',
        time: {
          unit: 'second',
        },
        distribution: 'linear',
      }],
      yAxes: [{
        ticks: {
          beginAtZero: true,
        },
      }],
    },
  },
});

// Display the most recent temperature reading and log to console
const latestTemperatureElement = document.getElementById('latestTemperature');

// Fetch and update graph data every few seconds
function fetchDataAndUpdateGraph() {
  fetch('/api/temperature')
    .then(response => response.json())
    .then(data => {
      const timestamp = new Date().toLocaleTimeString();
      const temperature = data.temperature; // Modify this based on your API response

      // Log data to console
      console.log('Received Data - Timestamp:', timestamp, 'Temperature:', temperature);

      // Update the latest temperature element
      latestTemperatureElement.textContent = `Latest Temperature: ${temperature} °C`;

      // Update chart data
      temperatureChart.data.labels.push(timestamp);
      temperatureChart.data.datasets[0].data.push(temperature);

      temperatureChart.update();
    })
    .catch(error => {
      console.error('Error fetching data:', error);
    });
}

setInterval(fetchDataAndUpdateGraph, 5000); // Fetch data every 5 seconds
