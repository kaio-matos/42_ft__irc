const url = process.argv[2]


const promise1 = fetch(url, {

	headers: {
		SLOW: 'true',
		'ALO': 'aff'
	},

	method: 'GET'
}).catch(() => { })
const promise2 = fetch(url).catch(() => { })

const r1 = await promise1
const r2 = await promise2

const result = await Promise.all([r1?.text(), r2?.text()])

console.log(result)
