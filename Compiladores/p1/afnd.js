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
const automata = crearAutomata(archivo);

const caminosRecorridos = [];
caminosRecorridos.push({ recorrido: [automata.inicial], actual: automata.inicial, errores: [] });

cadena.forEach(simbolo => {
	let nuevosCaminos = [];
	for(let camino of caminosRecorridos){
		//Buscar entre todas las transiciones aquellas que correspondan con el estado actual
		//de nuestro camino actual y nuestro simbolo actual
		let transiciones = automata.transiciones.filter(trans => {
			return trans.actual === camino.actual && trans.simbolo === simbolo
		});

		if (transiciones.length === 0) {
			//Agregar el error a nuestro camino
			camino.errores.push({ estado: camino.actual, simbolo });
			//Ir al siguiente camino recorrido
			//Si es el último camino recorrido, entonces ir al siguiente símbolo
			continue;
		}

		//Si hay sólo una transición, actualizar el camino actual
		if(transiciones === 1){
			camino.actual = transiciones[0].siguiente;
			camino.recorrido.push(camino.actual);
		}
		//Si no, crear nuevos caminos...
		else{
			for (let i = 1; i < transiciones.length; i++) {
				//... con base en el camino actual...
				let nuevoCamino = {
					recorrido: [...camino.recorrido],
					actual: camino.actual,
					errores: [...camino.errores]};

				nuevoCamino.actual = transiciones[i].siguiente;
				nuevoCamino.recorrido.push(nuevoCamino.actual);
				nuevosCaminos.push(nuevoCamino);
			}
			//... y actualizar el camino actual
			camino.actual = transiciones[0].siguiente;
			camino.recorrido.push(camino.actual);
		}
	}

	//Añadir los nuevos caminos al finalizar de recorrer todos los caminos con el símbolo actual
	caminosRecorridos.push(...nuevosCaminos);
});

console.log("\nFin. Estados recorridos: ");
caminosRecorridos.forEach(camino => {
	console.log("\nCamino:");
	console.log(camino.recorrido);
	//Imprimir errores
	camino.errores.forEach(error => {
		console.log(`Error en ${error.estado} con el símbolo ${error.simbolo}`);
	});
})

function crearAutomata(archivo){
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

	//Completar transiciones
	const nuevasTransiciones = [];
	//Añadir estado de error
	estados.push("\u274c");
	estados.forEach(estado => {
		alfabeto.forEach(simbolo => {
			//Iterar a través del alfabeto y buscar si existe una transición para el estado actual con el símbolo actual
			let validas = transiciones.filter(trans => trans.simbolo === simbolo && trans.actual === estado);
			//Si no existe esa o esas transiciones, agregar una al estado de error
			if(validas < 1){
				nuevasTransiciones.push({ actual: estado, simbolo, siguiente: "\u274c"});
			}
		});
	});

	console.log("Transiciones agregadas: ");
	nuevasTransiciones.forEach(trans => console.log(trans));
	transiciones.push(...nuevasTransiciones);

	//Formar quintupla con la información del archivo
	return { estados, alfabeto, inicial, finales, transiciones };
}