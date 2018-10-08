const fs = require("fs");

if(!process.argv[2]){
	console.error("Falta la ruta del archivo");
	process.exit(-1);
}

if(!process.argv[3]){
	console.error("Falta la cadena a probar");
	process.exit(-1);
}

const ruta = process.argv[2];
const cadena = process.argv[3].split("");
const archivo = (fs.readFileSync(ruta)).toString().split(/\r\n/);
const automata = iniciarAutomata(archivo);

const estadosRecorridos = [];
let estadoActual = automata.inicial;
estadosRecorridos.push(estadoActual);

cadena.forEach(simbolo => {
	//Buscar entre todas las transiciones que corresponden con nuestro estado actual
	let transiciones = automata.transiciones.filter(trans => trans.actual === estadoActual);

	//Buscar aquella transición que incluye a nuestro símbolo actual
	let transicion = transiciones.find(trans => trans.simbolo === simbolo);

	//Establecer nuevo estado actual
	estadoActual = transicion.siguiente;

	//Agregar el estado actual a la lista de estados recorridos
	estadosRecorridos.push(estadoActual);
});

console.log("Fin. Estados recorridos: ");
console.log(estadosRecorridos);

function iniciarAutomata(archivo){
	//Obtener estados
	const estados = [];
	archivo[0].split(",").forEach(elem => {
		estados.push(elem);
	});

	//Obtener alfabeto
	const alfabeto = [];
	archivo[1].split("").forEach(elem => {
		alfabeto.push(elem);
	});

	//Obtener estado inicial
	const inicial = archivo[2];

	//Obtener estados finales
	const finales = [];
	archivo[3].split(",").forEach(elem => {
		finales.push(elem);
	});

	//Obtener transiciones
	const transiciones = [];
	for (let i = 4; i < archivo.length; i++) {
		let linea = archivo[i].split(",");
		let actual = linea[0];
		let simbolo = linea[1];
		let siguiente = linea[2];
		transiciones.push({ actual, simbolo, siguiente });
	}

	return { estados, alfabeto, inicial, finales, transiciones }
}