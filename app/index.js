const express = require('express');
const Influx = require('influx');

const app = express();
const port = process.env.PORT || 3000;

// Use environment variables for InfluxDB configuration
const influx = new Influx.InfluxDB({
  host: process.env.INFLUXDB_HOST,
  database: process.env.INFLUXDB_DB,
  port: process.env.INFLUXDB_PORT,
  username: process.env.INFLUXDB_USER,
  password: process.env.INFLUXDB_PASSWORD,
  schema: [
    {
      measurement: 'your_measurement_name',
      fields: {
        value: Influx.FieldType.FLOAT,
      },
      tags: ['tag_name'],
    },
  ],
});

app.use(express.json());

app.get('/', (req, res) => {
  // Query InfluxDB to get database information
  influx
    .query('SHOW DATABASES')
    .then((result) => {
      const databases = result.map((row) => row.name);
      res.json({ databases });
    })
    .catch((err) => {
      console.error(`Error querying database information from InfluxDB: ${err.message}`);
      res.status(500).send('Internal Server Error');
    });
});

app.listen(port, () => {
  console.log(`Server is running on http://localhost:${port}`);
});
