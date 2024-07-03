import { CreateClient, SUCCESS } from "./tools.mjs";

const kaio = await CreateClient("kaio", "kaio");
const john = await CreateClient("john", "john");

await kaio.send("JOIN #games");

kaio.connection.on("data", (buffer) => {
  const response = buffer.toString();

  if (
    response.toLowerCase().includes("new user john has entered the channel")
  ) {
    kaio.send("KICK john #games\n");
  }

  if (response.toLowerCase().includes("john got kicked")) {
    SUCCESS();
  }
});

await john.send("JOIN #games");
