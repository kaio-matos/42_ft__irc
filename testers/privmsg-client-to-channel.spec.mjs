import { CreateClient, SUCCESS } from "./tools.mjs";

const kaio = await CreateClient("kaio", "kaio");
const john = await CreateClient("john", "john");
await kaio.send("JOIN #games");
await john.send("JOIN #games");

const messageSent = "Hello are you ALL receiving this message?";

john.connection.on("data", (buffer) => {
  const response = buffer.toString();

  if (response.includes(messageSent)) {
    SUCCESS();
  }
});

await kaio.send("PRIVMSG #games :" + messageSent);
