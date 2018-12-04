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

let inicial = {
	simbolo: String.fromCharCode(codigoEstado),
	estadosThompson: (cerraduraE([automata.inicial], automata.transiciones)).ordenar(),
	letra: []
};
afd.estados.push(inicial);
afd.inicial = inicial.simbolo;

//Recorrer cada nuevo estado que vaya apareciendo
for(let estado of afd.estados){
	//Y ejecutando irA() con cada letra del alfabeto
	for(let letra of afd.alfabeto){
		let nuevoEstado = irA(estado.estadosThompson, letra, automata.transiciones);
		//Ordenar ascendentemente los estados (convierte el Set a arreglo)
		nuevoEstado = nuevoEstado.ordenar();

		//Buscar si ya existe ese estado
		let iguales = afd.estados.filter(estado => compararSets(nuevoEstado, estado.estadosThompson));

		//Si no existe...
		if(iguales.length === 0){
			//Agregar la letra del alfabeto al estado actual
			//para saber con qué letras tiene transición.
			//Sólo es con el propósito de depurar
			estado.letra.push(letra);
			let estadoAFD = {
				simbolo: String.fromCharCode(++codigoEstado),
				estadosThompson: nuevoEstado,
				letra: []
			};

			afd.transiciones.push({
				actual: estado.simbolo,
				letra,
				siguiente: estadoAFD.simbolo
			});

			//Si tiene algún estado que forme parte de los estados finales del ANFD-E...
			//Entonces agregarlo a los estados finales del AFD
			for (let final of automata.finales) {
				if (nuevoEstado.includes(final)) {
					afd.finales.push(estadoAFD.simbolo);
				}
			}

			//Añadir a la lista de estados
			afd.estados.push(estadoAFD);
		}
		//Si sí existe...
		else{
			//Y no es el estado vacío...
			if(iguales[0].simbolo !== "-"){
				//Crear una nueva transición
				afd.transiciones.push({
					actual: estado.simbolo,
					letra,
					siguiente: iguales[0].simbolo
				});

				//Y también añadir el símbolo actual al estado actual
				estado.letra.push(letra);
			}
		}
	}
}

//Nota: es hora de aprender a usar util.format()
let est = [];
for(let i = 1; i < afd.estados.length; i++){
	est.push(afd.estados[i].simbolo);
}
console.log(`Estados: [${est.join(", ")}]`);
console.log(`Alfabeto: [${afd.alfabeto.join(", ")}]`);
console.log(`Inicial: ${afd.inicial}`);
console.log(`Estados finales: [${afd.finales.join(", ")}]`);
console.log("Transiciones: ");
console.log(afd.transiciones);

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
