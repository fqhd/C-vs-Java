function setup(){
	fetch('output.json').then(response => response.json()).then(json => {
		const arr = json.array;
		const width = Math.sqrt(arr.length);
		console.log('Width: ' + width);
		createCanvas(width, width);
		background(0);

		for(let y = 0; y < width; y++){
			for(let x = 0; x < width; x++){
				const c = color(arr[y * width + x] * 255);
				set(x, y, c);
			}
		}
		updatePixels();
	});
}

function draw(){
}