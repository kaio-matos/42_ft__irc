import net from "node:net";

const cKaio = net.createConnection(
  { localAddress: "0.0.0.0", port: 8080 },
  function onConnect() {
    cKaio.write("USER kaio kaio\n");
    cKaio.on("data", (buffer) => {
      const response = buffer.toString();

      console.log("\nUser Kaio ---------------------------------");
      console.log(response);

      if (
        response.toLowerCase().includes("new user john has entered the channel")
      ) {
        cKaio.write("KICK john games\n");
      }

      console.log("User Kaio ---------------------------------\n");
    });
  },
);

await new Promise((res) => setTimeout(res, 500));

const cJohn = net.createConnection(
  { localAddress: "0.0.0.0", port: 8080 },
  function onConnect() {
    cJohn.write("USER john john\n");

    cJohn.on("data", (buffer) => {
      const response = buffer.toString();

      console.log("\nUser John ---------------------------------");
      console.log(response);
      console.log("User John ---------------------------------\n");
    });
  },
);
