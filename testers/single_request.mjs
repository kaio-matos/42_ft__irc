const url = process.argv[2]

const response = await fetch(url)
const result = await response.text()
console.log(result)

