const limits = []
data = []

const rows = [
    { cols: 4, heightPercentage: 33.33 },  
    { cols: 5, heightPercentage: 33.33 },  
    { cols: 4, heightPercentage: 33.34 }   
];

function define_limits() {
    let currentY = 0;
    rows.forEach(row => {
        const { cols, heightPercentage } = row;
        const rowHeight = heightPercentage;
        const colWidth = 100 / cols;

        for (let j = 0; j < cols; j++) {
            limits.push({
                minx: j * colWidth,
                maxx: (j + 1) * colWidth,
                miny: currentY,
                maxy: currentY + rowHeight
            });
        }

        currentY += rowHeight;
    });
}

function generate_points(numberOfPoints) {
    for (let i = 0; i < limits.length; i++) {
        const { minx, maxx, miny, maxy } = limits[i];
        for (let j = 0; j < numberOfPoints; j++) {
            const xPos = Math.random() * (maxx - minx) + minx;
            const yPos = Math.random() * (maxy - miny) + miny;
            data.push({ x:xPos, y:yPos , className:i});
        }
    }
}

const colors = ['#ff0000', '#00ff00', '#0000ff', '#ffff00', '#ff00ff', '#00ffff', '#ff7f50', '#6495ed', '#333330', '#00a080', '#bdb76b', '#7f7f7f', '#ff1493'];


function getColor(index) {
    return colors[index % colors.length];
}

define_limits();
generate_points(120);


const canvas = d3.select(".canva")

const svg = canvas.append("svg")
            .attr("width","100%")
            .attr("height","100%")

const points = svg.selectAll("circle")
points.data(data)
        .enter().append("circle")
        .attr("cx",(d)=>d.x+'%')
        .attr("cy",(d)=>d.y+'%')
        .attr("r",7.5)
        .attr("fill", (d, i) => getColor(d.className));