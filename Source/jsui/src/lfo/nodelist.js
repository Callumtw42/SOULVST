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
            radius: pointRadius,
            isSelected: false
        }
    }
    updateControlPoint() {
        const startX = this.startNode.x;
        const startY = this.startNode.y;
        const endX = this.endNode.x;
        const endY = this.endNode.y;
        const dx = endX - startX;
        const dy = endY - startY;
        const relY = this.controlPoint.relY;

        this.controlPoint.x = startX + dx * 0.5;
        this.controlPoint.y = startY + dy * Math.abs(relY);
    }


    quadraticBezier(p0, p1, p2, t, pFinal) {
        pFinal = pFinal || {};
        pFinal.x = Math.pow(1 - t, 2) * p0.x +
            (1 - t) * 2 * t * p1.x +
            t * t * p2.x;
        pFinal.y = Math.pow(1 - t, 2) * p0.y +
            (1 - t) * 2 * t * p1.y +
            t * t * p2.y;
        return pFinal;
    }

    cubicBezier(p0, p1, p2, p3, t, pFinal) {
        pFinal = pFinal || {};
        pFinal.x = Math.pow(1 - t, 3) * p0.x +
            Math.pow(1 - t, 2) * 3 * t * p1.x +
            (1 - t) * 3 * t * t * p2.x +
            t * t * t * p3.x;
        pFinal.y = Math.pow(1 - t, 3) * p0.y +
            Math.pow(1 - t, 2) * 3 * t * p1.y +
            (1 - t) * 3 * t * t * p2.y +
            t * t * t * p3.y;
        return pFinal;
    }

    moveControlPoint(y) {
        const start = this.startNode;
        const end = this.endNode;
        // const endY = this.endNode.y;
        // const startY = this.startNode.y;
        const maxY = Math.max(start.y, end.y);
        const minY = Math.min(start.y, end.y);
        const clampY = clamp(y, minY, maxY);

        const dy = end.y - start.y;
        const yInBounds = clampY - end.y;
        // this.controlPoint.relY = (dy > 0) ? (maxY - yInBounds) / dy : -yInBounds / dy;
        this.controlPoint.relY = (-yInBounds * 2 / dy) - 1;

        const relY = this.controlPoint.relY;
        const dx = end.x - start.x;
        const ctrl1 = (relY >= 0)
            ? {
                x: start.x + dx * relY,
                y: start.y
            }
            : {
                x: start.x,
                y: start.y - dy * relY
            }

        const ctrl2 = (relY >= 0)
            ? {
                x: end.x,
                y: end.y - dy * relY
            }
            : {
                x: end.x + dx * relY,
                y: end.y
            }

        const midPoint = this.cubicBezier(start, ctrl2, ctrl1, end, 0.5);
        this.controlPoint.y = clampY; 
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
