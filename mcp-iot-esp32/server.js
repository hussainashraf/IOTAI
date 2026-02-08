import express from "express";
import fetch from "node-fetch";

const app = express();
app.use(express.json());

const ESP32_IP = "http://192.168.1.14"; // CHANGE THIS

// -------- MCP TOOL: Get status --------
app.get("/tools/get_intrusion_status", async (req, res) => {
  try {
    const response = await fetch(`${ESP32_IP}/status`);
    const data = await response.json();
    res.json(data);
  } catch (err) {
    res.status(500).json({ error: "ESP32 not reachable" });
  }
});

// -------- MCP TOOL METADATA --------
app.get("/tools", (req, res) => {
  res.json([
    {
      name: "get_intrusion_status",
      description: "Returns real-time obstacle detection state from ESP32",
      input_schema: {},
      output_schema: {
        sensor: "string",
        state: "string",
        uptime_ms: "number"
      }
    }
  ]);
});

app.post("/tools/set_alert_state", async (req, res) => {
  const { action } = req.body;

  await fetch(`${ESP32_IP}/command`, {
    method: "POST",
    headers: { "Content-Type": "application/x-www-form-urlencoded" },
    body: `action=${action}`
  });

  res.json({ status: "sent" });
});

app.listen(3000, () => {
  console.log("MCP bridge running on http://localhost:3000");
});
