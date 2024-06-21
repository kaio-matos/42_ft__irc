import net from "node:net";
import { exit } from "node:process";

setTimeout(() => exit(), 1000);

const messageSent = "Hello are you receiving this message?";

const cKaio = net.createConnection(
  { localAddress: "0.0.0.0", port: 8080 },
  function onConnect() {
    cKaio.write("USER kaio * 0 kaio\n");
    cKaio.on("data", (buffer) => {
      const response = buffer.toString();
    });
  },
);

await new Promise((res) => setTimeout(res, 500));

const cJohn = net.createConnection(
  { localAddress: "0.0.0.0", port: 8080 },
  function onConnect() {
    cJohn.write("USER john * 0 john\n", () => {
      cKaio.write("PRIVMSG john :" + messageSent + "\n");
    });

    cJohn.on("data", (buffer) => {
      const response = buffer.toString();

      if (response.includes(messageSent)) {
        console.log("SUCCESS");
      }
    });
  },
);
