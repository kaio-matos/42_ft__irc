import readline from "readline";
import { exit } from "node:process";
import { CreateClient } from "./tools.mjs";

const client = await CreateClient(process.argv[2], process.argv[3], false);
await client.send("JOIN #games");

// client.connection.on("end", (buffer) => {
//   console.log("Server closed the connection")
//   exit();
// });

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
});

client.connection.on("data", (buffer) => {
  const response = buffer.toString();
  console.log(
    response
      .split("\n")
      .map((s) => "<- |" + s)
      .join("\n"),
  );
});

function askCommand() {
  rl.question("", async function (command) {
    await client.send(command);

    askCommand();
  });
}
askCommand();
