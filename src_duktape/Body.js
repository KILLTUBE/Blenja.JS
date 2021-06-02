class Body {
    appendChild(child) {
        console.log("Body#appendChild", child);
    }
}

class Document {
    constructor() {
        this.body = new Body;
    }
}

document = new Document;
