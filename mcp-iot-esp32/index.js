import fetch from "node-fetch";

import { Server } from "@modelcontextprotocol/sdk/server/index.js";
import { StdioServerTransport } from "@modelcontextprotocol/sdk/server/stdio.js";

// 👇 IMPORTANT: these are the schemas MCP expects
import {
  ListToolsRequestSchema,
  CallToolRequestSchema,
} from "@modelcontextprotocol/sdk/types.js";

const ESP32_IP = "http://192.168.1.14"; // your ESP32 IP

// ---------------- MCP SERVER ----------------
const server = new Server(
  {
    name: "esp32-intrusion-system",
    version: "1.0.0",
  },
  {
    capabilities: {
      tools: {},
    },
  }
);

// ---------------- tools/list ----------------
server.setRequestHandler(ListToolsRequestSchema, async () => {
  return {
    tools: [
      {
        name: "get_intrusion_status",
        description: "Get real-time obstacle detection status from ESP32",
        inputSchema: {
          type: "object",
          properties: {},
        },
      },
      {
        name: "set_alert_state",
        description: "Control ESP32 alert state",
        inputSchema: {
          type: "object",
          properties: {
            action: {
              type: "string",
              enum: ["alert", "clear"],
            },
          },
          required: ["action"],
        },
      },
    ],
  };
});

// ---------------- tools/call ----------------
server.setRequestHandler(CallToolRequestSchema, async (request) => {
  const { name, arguments: args } = request.params;

  // ---- read sensor ----
  if (name === "get_intrusion_status") {
    const res = await fetch(`${ESP32_IP}/status`);
    const data = await res.json();

    return {
      content: [
        {
          type: "text",
          text: JSON.stringify(data, null, 2),
        },
      ],
    };
  }

  // ---- control hardware ----
  if (name === "set_alert_state") {
    const { action } = args;

    await fetch(`${ESP32_IP}/command`, {
      method: "POST",
      headers: {
        "Content-Type": "application/x-www-form-urlencoded",
      },
      body: `action=${action}`,
    });

    return {
      content: [
        {
          type: "text",
          text: `Action '${action}' sent to ESP32`,
        },
      ],
    };
  }

  throw new Error(`Unknown tool: ${name}`);
});

// ---------------- START SERVER ----------------
const transport = new StdioServerTransport();
await server.connect(transport);
