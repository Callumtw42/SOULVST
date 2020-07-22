
export default class NodeList {
    constructor(head, tail) {
        this.list = [head, tail];
        head.rightNeighbour = tail;
        head.leftNeighbour = null;
        tail.rightNeighbour = null;
        tail.leftNeighbour = head;
    }

    insertAfter(index, node) {
        this.list.splice(
            index + 1,
            0,
            node
        )
        node.leftNeighbour = this.list[index];
        node.leftNeighbour.rightNeighbour = node;

        node.rightNeighbour = (this.list[index + 2]) ? this.list[index + 2] : null;
        node.rightNeighbour.leftNeighbour = node;
    }

    remove(index) {
        const point = this.list[index];
        const leftNeighbour = point.leftNeighbour;
        const rightNeighbour = point.rightNeighbour;

        this.list.splice(index, 1);
        this.list[index].leftNeighbour = leftNeighbour;
        this.list[index - 1].rightNeighbour = rightNeighbour;
    }

    forEach(...args) {
        return this.list.forEach(...args);
    }

    indexOf(...args) {
        return this.list.indexOf(...args);
    }

    splice(...args) {
        return this.list.splice(...args);
    }

    map(...args) {
        return this.list.map(...args);
    }

    get() {
        return this.list;
    }
}
