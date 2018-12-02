const fs = require("fs");

if (!process.argv[2]) {
	console.error("Falta la ruta del archivo");
	process.exit(-1);
}

Set.prototype.ordenar = function() {
	return [...this].sort((a, b) => a - b);
}

const ruta = process.argv[2];
const archivo = (fs.readFileSync(ruta)).toString().split(/\r?\n/);
const automata = crearAutomata(archivo);
const afd = {
	estados: [{
		simbolo: "-",
		estadosThompson: [],
		letra: []
	}],
	alfabeto: automata.alfabeto,
	inicial: null,
	finales: [],
	transiciones: []
};

let codigoEstado = 65;

afd.estados.push({ 
	simbolo: String.fromCharCode(codigoEstado),
	estadosThompson: (cerraduraE([automata.inicial], automata.transiciones)).ordenar(),
	letra: []
});


for(let estado of afd.estados){
	for(let simbolo of afd.alfabeto){
		let nuevoEstado = irA(estado.estadosThompson, simbolo, automata.transiciones);
		nuevoEstado = nuevoEstado.ordenar();

		let similares = afd.estados.filter(estado => compararSets(nuevoEstado, estado.estadosThompson));
		if(similares.length === 0){
			estado.letra.push(simbolo);
			afd.estados.push({
				simbolo: String.fromCharCode(++codigoEstado),
				estadosThompson: nuevoEstado,
				letra: []
			});
		}
		else{
			if(similares[0].simbolo !== "-"){
				estado.letra.push(simbolo);
			}
		}
	}
}

function cerraduraE(estados, transiciones) {
	let resultado = new Set([...estados]);
	
	for(let estado of resultado){
		let nuevosEstados = transiciones.filter(trans => {
			return (trans.actual === estado) && (trans.simbolo === "E");
		});
		for(let nuevo of nuevosEstados){
			resultado.add(nuevo.siguiente);
		}
	}

	return resultado;
}

function mover(estados, simbolo, transiciones) {
	let resultado = new Set();

	for (let estado of estados) {
		let nuevosEstados = transiciones.filter(trans => {
			return (trans.actual === estado) && (trans.simbolo === simbolo);
		});
		for (let nuevo of nuevosEstados) {
			resultado.add(nuevo.siguiente);
		}
	}

	return resultado;
}

function irA(estados, simbolo, transiciones) {
	return cerraduraE(mover(estados, simbolo, transiciones), transiciones);
}

function compararSets(nuevoEstado, estadosThompson){
	if (estadosThompson.length !== nuevoEstado.length) {
		return false;
	}

	for (let i = 0; i < nuevoEstado.length; i++) {
		if (estadosThompson[i] !== nuevoEstado[i]) {
			return false;
		}
	}

	return true;
}

function crearAutomata(archivo) {
	//Obtener estados
	const estados = [];
	archivo[0].split(",").forEach(elem => {
		estados.push(elem);
	});

	//Obtener alfabeto
	const alfabeto = [];
	archivo[1].split(",").forEach(elem => {
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


	//Formar quintupla con la información del archivo
	return { estados, alfabeto, inicial, finales, transiciones };
}
