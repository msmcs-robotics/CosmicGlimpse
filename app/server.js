const express = require('express');
const Influx = require('influx');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3000;

// Define your InfluxDB configuration
const influx = new Influx.InfluxDB({
  host: process.env.INFLUXDB_HOST, // Updated to use the host name defined in Docker Compose
  port: process.env.INFLUXDB_PORT,
  database: process.env.INFLUXDB_DB,
  username: process.env.INFLUXDB_USER,
  password: process.env.INFLUXDB_PASSWORD,
  schema: [
    {
      measurement: 'your_measurement_name', // Replace with your actual measurement name
      fields: {
        'fgm-measure': Influx.FieldType.FLOAT,
      },
      tags: ['device-id'],
    },
  ],
});

// Check if the database exists and create it if not
influx.getDatabaseNames()
  .then(names => {
    if (!names.includes(process.env.INFLUXDB_DB)) {
      return influx.createDatabase(process.env.INFLUXDB_DB);
    }
  })
  .catch(error => {
    console.error('Error creating database:', error);
  });

app.use(express.json()); // Parse JSON request bodies
app.use(express.static(path.join(__dirname, 'public')));

app.get('/', async (req, res) => {
  try {
    // Query data from your measurement
    const result = await influx.query('SELECT * FROM your_measurement_name');

    // Extract the data for visualization
    const timestamps = result.map(row => row.time);
    const fgmMeasures = result.map(row => row['fgm-measure']); // Correct field name

    // Render the data visualization
    res.json({ timestamps, fgmMeasures });
  } catch (error) {
    console.error(error);
    res.status(500).send('Error reading from InfluxDB');
  }
});

app.post('/add-entry', async (req, res) => {
  // Extract data from the request body
  const { timestamp, deviceId, fgmMeasure } = req.body;

  // Add an entry with the specified data
  const newEntry = {
    measurement: 'your_measurement_name',
    fields: { 'fgm-measure': fgmMeasure },
    tags: { 'device-id': deviceId },
    timestamp: new Date(timestamp), // Ensure timestamp is a valid date
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
