const express = require("express");
const bodyParser = require("body-parser");

const app = express();
const PORT = 3000;

// Middleware to parse JSON
app.use(bodyParser.json());

// Route to handle Arduino data
app.get("/hardware/change-state", (req, res) => {
  console.log("Data received from Arduino:", req.body);

  // Respond back to Arduino
  res.json({ state: ["taken", "taken", "taken", "taken"] });
});

app.post("/hardware/new-intake", (req, res) => {
  console.log("Data received from Arduino:", req.body);

  res.json({ message: "Data received successfully", status: "OK" });
});

app.post("/hardware/keep-opening", (req, res) => {
  console.log("Data received from Arduino:", req.body);

  res.json({ message: "Data received successfully", status: "OK" });
});

// Start server
app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});