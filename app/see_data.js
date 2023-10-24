// Import the necessary libraries
const Influx = require('influx');
const Chart = require('chart.js');
const express = require('express');
const path = require('path');
const app = express();

// Define your InfluxDB configuration
const influx = new Influx.InfluxDB({
  host: 'influxdb',  // Use the service name defined in your Docker Compose
  database: 'mydb', // Change this to your database name
});

// Set up your web server
const PORT = process.env.PORT || 3000;

app.use(express.static(path.join(__dirname, 'public')));

app.get('/data', async (req, res) => {
  try {
    // Query data from your measurement
    const result = await influx.query('SELECT * FROM your_measurement_name');

    // Extract the data for visualization
    const timestamps = result.map(row => row.time);
    const fgmMeasures = result.map(row => row.fgm_measure);

    // Render the data visualization
    res.json({ timestamps, fgmMeasures });
  } catch (error) {
    console.error(error);
    res.status(500).send('Error reading from InfluxDB');
  }
});

app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
