const fs = require("fs");

if (!process.argv[2]) {
	console.error("Falta la ruta del archivo");
	process.exit(-1);
}

const ruta = process.argv[2];
const cadena = process.argv[3].split("");
const archivo = (fs.readFileSync(ruta)).toString().split(/\r?\n/);
const automata = crearAutomata(archivo);

const caminosRecorridos = [];
caminosRecorridos.push({ 
	recorrido: [automata.inicial], 
	actual: automata.inicial, 
	errores: [],
	indiceActual: 0 });

while (!caminosRecorridos.every(hue)) {
	let nuevosCaminos = [];
	for (let i = 0; i < caminosRecorridos.length; i++) {
		let camino = caminosRecorridos[i];
		let simbolo = cadena[camino.indiceActual];

		let transSigma = automata.transiciones.find(trans => {
			return trans.actual === camino.actual && trans.simbolo === simbolo;
		});

		let transEpsi = automata.transiciones.filter(trans => {
			return trans.actual === camino.actual && trans.simbolo === "E";
		});

		//Hay transición con el elemento actual
		if(transSigma){
			camino.actual = transSigma.siguiente;
			camino.recorrido.push(camino.actual);
			++camino.indiceActual;
			continue;
		}

		//No hay, intentar con epsilon
		//Si no hay transiciones con epsilon
		if(transEpsi.length === 0){
			if(!automata.finales.includes(camino.actual)){
				caminosRecorridos.splice(i, 1);
			}
			continue;
		}

		//Hay transicion con epsilon
		if (transEpsi === 1) {
			camino.actual = transiciones[0].siguiente;
			camino.recorrido.push(camino.actual);
		}
		//Si no, crear nuevos caminos...
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





		//Se acabó la cadena (no símbolo)
		//Probar con epsilon
		//Símbolo no pertenece al alfabeto


	}

	caminosRecorridos.push(...nuevosCaminos);	
}

console.log(`\nFin, se recorrieron ${caminosRecorridos.length} camino(s)`);
caminosRecorridos.forEach(camino => {
	console.log(camino);
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

	/*
	//Completar transiciones
	const nuevasTransiciones = [];
	//Añadir estado de error
	estados.push("\u274c");
	estados.forEach(estado => {
		alfabeto.forEach(simbolo => {
			//Iterar a través del alfabeto y buscar si existe una transición para el estado actual con el símbolo actual
			let validas = transiciones.filter(trans => trans.simbolo === simbolo && trans.actual === estado);
			//Si no existe esa o esas transiciones, agregar una al estado de error
			if (validas < 1) {
				nuevasTransiciones.push({ actual: estado, simbolo, siguiente: "\u274c" });
			}
		});
	});

	console.log("Transiciones agregadas: ");
	nuevasTransiciones.forEach(trans => console.log(trans));
	transiciones.push(...nuevasTransiciones);*/

	//Formar quintupla con la información del archivo
	return { estados, alfabeto, inicial, finales, transiciones };
}

function hue(elemento) {
	return automata.finales.includes(elemento.actual);
}