import net from "node:net";
import { exit } from "node:process";

/**
 * @param {string} nickname
 * @param {string} username
 */
export async function CreateClient(nickname, username, timeout = true) {
  /** @type {import('node:net').Socket} */
  const connection = await new Promise((res) => {
    const c = net.createConnection(
      { localAddress: "0.0.0.0", port: 8080 },
      function onConnect() {
        c.write(`PASS teste\r\n`, () => res(c));
        c.write(`USER ${nickname} * 0 ${username}\r\n`, () => res(c));
      },
    );
  });

  if (timeout) {
    setTimeout(() => {
      console.log("Timed out");
      exit();
    }, 1500);
  }

  return {
    connection,
    nickname,
    username,

    /**
     * @param {string} message
     */
    async send(message) {
      await new Promise((res) =>
        connection.write(message + "\r\n", () => res()),
      );

      return delay(100);
    },
  };
}

export function delay(time = 500) {
  return new Promise((res) => setTimeout(res, time));
}

export function SUCCESS() {
  console.log("SUCCESS");
  exit();
}
