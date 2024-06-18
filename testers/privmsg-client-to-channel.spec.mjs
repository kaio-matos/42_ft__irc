import net from "node:net";

const cKaio = net.createConnection(
  { localAddress: "0.0.0.0", port: 8080 },
  function onConnect() {
    cKaio.write("USER kaio * 0 kaio\n");
    cKaio.on("data", (buffer) => {
      const response = buffer.toString();

      console.log("\nUser Kaio ---------------------------------");
      console.log(response);

      if (
        response.toLowerCase().includes("new user john has entered the channel")
      ) {
        cKaio.write(
          "PRIVMSG Games :Hello are you ALL receiving this message?\n",
        );
      }
      console.log("User Kaio ---------------------------------\n");
    });
  },
);

const cJohn = net.createConnection(
  { localAddress: "0.0.0.0", port: 8080 },
  function onConnect() {
    cJohn.write("USER john * 0 john\n");

    cJohn.on("data", (buffer) => {
      const response = buffer.toString();

      console.log("\nUser John ---------------------------------");
      console.log(response);
      console.log("User John ---------------------------------\n");
    });
  },
);
