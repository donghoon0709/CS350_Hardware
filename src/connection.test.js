const express = require("express");
const bodyParser = require("body-parser");

const app = express();
const PORT = 3000;

let isTaken = [false,false,false,false];
let isKeepOpening = [false,false,false,false];

// Middleware to parse JSON
app.use(bodyParser.json());

app.get("/ping", (req, res) => {
  console.log("Received ping from Arduino");
  res.send("pong"); // Respond with "pong"
});


// Route to handle Arduino data
app.get("/api/hardware/change-state", (req, res) => {
  console.log("Data received from Arduino:", req.body);

  // Respond back to Arduino
  res.json({ state: isTaken });
});

app.post("/hardware/new-intake", (req, res) => {
  console.log("Data received from Arduino:", req.body);
  const { newIntakeArray } = req.body;

  isTaken = newIntakeArray;

  console.log("Updated isTaken array:", newIntakeArray);
  res.json({ message: "Array updated successfully", status: "OK" });
});

app.post("/hardware/keep-opening", (req, res) => {
  console.log("Data received from Arduino:", req.body);

  const { keepOpeningArray } = req.body;

  if (!Array.isArray(keepOpeningArray) || keepOpeningArray.length !== isKeepOpening.length) {
    return res.status(400).json({ message: "Invalid array format or length", status: "Error" });
  }

  // 배열 업데이트
  isKeepOpening = keepOpeningArray;

  console.log("Updated isKeepOpening array:", isKeepOpening);
  res.json({ message: "Array updated successfully", status: "OK" });
});

// Start server
app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});