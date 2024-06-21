import net from "node:net";
import { exit } from "node:process";

setTimeout(() => exit(), 1000);

const cKaio = net.createConnection(
  { localAddress: "0.0.0.0", port: 8080 },
  function onConnect() {
    cKaio.write("USER kaio * 0 kaio\n", () => cKaio.write("JOIN #games\n"));

    cKaio.on("data", (buffer) => {
      const response = buffer.toString();

      if (
        response.toLowerCase().includes("new user john has entered the channel")
      ) {
        cKaio.write("KICK john #games\n");
      }

      if (response.toLowerCase().includes("john got kicked")) {
        console.log("SUCCESS");
        cKaio.end();
        cJohn.end();
      }
    });
  },
);

await new Promise((res) => setTimeout(res, 500));

const cJohn = net.createConnection(
  { localAddress: "0.0.0.0", port: 8080 },
  async function onConnect() {
    cJohn.write("USER john * 0 john\n", () => cJohn.write("JOIN #games\n"));

    cJohn.on("data", (buffer) => {
      const response = buffer.toString();
    });
  },
);
