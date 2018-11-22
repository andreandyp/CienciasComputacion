const fs = require("fs");

if (!process.argv[2]) {
	console.error("Falta la ruta del archivo");
	process.exit(-1);
}

const ruta = process.argv[2];
const cadena = (process.argv[3] || "").split("");
const archivo = (fs.readFileSync(ruta)).toString().split(/\r?\n/);
const automata = crearAutomata(archivo);

const caminosRecorridos = [];
caminosRecorridos.push({ 
	recorrido: [automata.inicial], 
	actual: automata.inicial, 
	errores: [],
	indiceActual: 0 });

//Mientras todos los caminos NO estén en un estado final
while (!caminosRecorridos.every(estaEnEstadoFinal)) {
	let nuevosCaminos = [];
	for (let i = 0; i < caminosRecorridos.length; i++) {
		let camino = caminosRecorridos[i];
		//Obtener el símbolo a leer de la cadena con base en el índice que guarde el camino actual
		let simbolo = cadena[camino.indiceActual];

		//Buscar SÓLO UNA transición con sigma (símbolo del alfabeto)
		let transSigma = automata.transiciones.find(trans => {
			return trans.actual === camino.actual && trans.simbolo === simbolo;
		});

		//Buscar las transiciones con épsilon
		let transEpsi = automata.transiciones.filter(trans => {
			return trans.actual === camino.actual && trans.simbolo === "E";
		});

		//Hay transición con el símbolo actual
		if(transSigma){
			//Actualizar el estado actual, el recorrido y el índice del camino actual
			//y pasar al siguiente camino
			camino.actual = transSigma.siguiente;
			camino.recorrido.push(camino.actual);
			++camino.indiceActual;
			continue;
		}

		//Si es un símbolo que no se encuentra en el alfabeto
		//Y no es undefined (épsilon en este caso), añadir un error y actualizar el índice actual
		if (!automata.alfabeto.includes(simbolo) && simbolo) {
			++camino.indiceActual;
			camino.errores.push({ estado: camino.actual, simbolo });
		}

		//No hay, intentar con epsilon
		//Si no hay transiciones con epsilon...
		if(transEpsi.length === 0){
			//Si ya no se puede avanzar y no está en un estado final, eliminar el camino
			if(!estaEnEstadoFinal(camino)){
				caminosRecorridos.splice(i, 1);
				continue;
			}
			//Y pasar al siguiente camino
			continue;
		}

		//Hay UNA transición con épsilon, actualizar el camino actual
		if (transEpsi.length === 1) {
			camino.actual = transEpsi[0].siguiente;
			camino.recorrido.push(camino.actual);
		}
		//Si hay más de una transición, crear nuevos caminos...
		else {
			for (let i = 1; i < transEpsi.length; i++) {
				//... con base en el camino actual...
				let nuevoCamino = {
					recorrido: [...camino.recorrido],
					actual: camino.actual,
					errores: [...camino.errores]
				};

				nuevoCamino.actual = transEpsi[i].siguiente;
				nuevoCamino.recorrido.push(nuevoCamino.actual);
				nuevoCamino.indiceActual = camino.indiceActual;
				nuevosCaminos.push(nuevoCamino);
			}
			//... y actualizar el camino actual
			camino.actual = transEpsi[0].siguiente;
			camino.recorrido.push(camino.actual);
		}
	}

	//Añadir los nuevos caminos (bifurcaciones)
	caminosRecorridos.push(...nuevosCaminos);	
}

console.log(`\nFin, se recorrieron ${caminosRecorridos.length} camino(s)`);
caminosRecorridos.forEach(camino => {
	console.log(`Recorrido: ${camino.recorrido}`);
	console.log("Errores:");
	camino.errores.forEach(error => {
		console.log(`Error en el estado ${error.estado} con el símbolo ${error.simbolo}`);
	});
})

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

function estaEnEstadoFinal(camino) {
	return automata.finales.includes(camino.actual);
}