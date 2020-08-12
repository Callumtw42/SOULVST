import { pointRadius, plotHeight } from "./lfo"
import { clamp } from "../functions"

class Path {
    constructor(startNode, endNode) {
        this.startNode = startNode;
        this.endNode = endNode;
        this.controlPoint = {
            relY: 0.5,
            x: startNode.x + (endNode.x - startNode.x) / 2,
            y: startNode.y + (startNode.y - endNode.y) / 2,
            radius: pointRadius - 1,
            isSelected: false//
        }
    }
    updateControlPoint() {
        this.controlPoint.x = this.startNode.x + (this.endNode.x - this.startNode.x) * this.controlPoint.relY;
        this.controlPoint.y = this.startNode.y + (this.endNode.y - this.startNode.y) * this.controlPoint.relY;
    }
    moveControlPoint(y) {
        const endY = this.endNode.y;
        const startY = this.startNode.y;
        const maxY = Math.max(startY, endY);
        const minY = Math.min(startY, endY);
        const clampY = clamp(y, minY, maxY);

        const dy = Math.abs(this.endNode.y - this.startNode.y);
        this.controlPoint.y = clampY;
        this.controlPoint.relY = clampY / (dy - Math.min(this.endNode.y, this.startNode.y));
    }
}

export class Node {
    constructor(x, y, radius, isBound) {

        this.x = x;
        this.y = y;
        this.radius = radius;
        this.isSelected = false;
        this.isBound = isBound;
        this.leftNeighbour = null;
        this.rightNeighbour = null;
        this.path = null;
    }

    move(x, y) {
        const clampY = clamp(y, 0, plotHeight);
        const clampX = (this.leftNeighbour && this.rightNeighbour)
            ? clamp(x, this.leftNeighbour.x, this.rightNeighbour.x)
            : this.x;

        this.y = clampY;
        if (!this.isBound)
            this.x = clampX;
        if (this.path) this.path.updateControlPoint();
        if (this.leftNeighbour) this.leftNeighbour.path.updateControlPoint();
    }
}

export class NodeList {
    constructor(head, tail) {
        head.path = new Path(head, tail)
        this.nodes = [head, tail];
        head.rightNeighbour = tail;
        head.leftNeighbour = null;
        tail.rightNeighbour = null;
        tail.leftNeighbour = head;
    }

    insertAfter(index, node) {

        this.nodes.splice(
            index + 1,
            0,
            node
        )
        node.leftNeighbour = this.nodes[index];
        node.leftNeighbour.rightNeighbour = node;
        node.rightNeighbour = (this.nodes[index + 2]) ? this.nodes[index + 2] : null;
        node.rightNeighbour.leftNeighbour = node;

        node.leftNeighbour.path = new Path(node.leftNeighbour, node);
        node.leftNeighbour.path.updateControlPoint();

        node.path = new Path(node, node.rightNeighbour)
        node.path.updateControlPoint();
    }

    remove(index) {
        const point = this.nodes[index];
        const leftNeighbour = point.leftNeighbour;
        const rightNeighbour = point.rightNeighbour;
        leftNeighbour.path = new Path(leftNeighbour, rightNeighbour);

        this.nodes.splice(index, 1);
        this.nodes[index].leftNeighbour = leftNeighbour;
        leftNeighbour.path.updateControlPoint();
        this.nodes[index - 1].rightNeighbour = rightNeighbour;

    }

    forEach(...args) {
        return this.nodes.forEach(...args);
    }

    indexOf(...args) {
        return this.nodes.indexOf(...args);
    }

    splice(...args) {
        return this.nodes.splice(...args);
    }

    map(...args) {
        return this.nodes.map(...args);
    }

    get() {
        return this.nodes;
    }
}
