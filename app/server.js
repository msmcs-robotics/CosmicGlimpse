const express = require('express');
const Influx = require('influx');
const Chart = require('chart.js');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3000;

// Define your InfluxDB configuration
const influx = new Influx.InfluxDB({
  host: 'influxdb',  // Use the service name defined in your Docker Compose
  database: 'mydb', // Change this to your database name
});

app.use(express.static(path.join(__dirname, 'public'));

app.get('/', async (req, res) => {
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

app.get('/add-entry', async (req, res) => {
  // Add an entry with the specified data
  const newEntry = {
    measurement: 'your_measurement_name',
    fields: { 'fgm-measure': '3-4-2' },
    tags: { 'device-id': 'ak-1' },
    timestamp: new Date().toISOString(),
  };

  try {
    await influx.writePoints([newEntry]);
    res.send('Entry added successfully');
  } catch (error) {
    console.error(error);
    res.status(500).send('Error writing to InfluxDB');
  }
});

app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
