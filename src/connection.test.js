const express = require("express");
const bodyParser = require("body-parser");

const app = express();
const PORT = 3000;

// Middleware to parse JSON
app.use(bodyParser.json());

// Route to handle Arduino data
app.post("/arduino-data", (req, res) => {
  console.log("Data received from Arduino:", req.body);

  // Respond back to Arduino
  res.json({ message: "Data received successfully", status: "OK" });
});

// Start server
app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});