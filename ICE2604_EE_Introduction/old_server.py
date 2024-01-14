from flask import Flask, request, render_template
import base

app = Flask(
    "dianlei",
    static_folder="./static",
    template_folder="./templates"
)

keyword =open('data/keyword.txt', 'r', encoding='utf-8').read().split() 

@app.route('/') 
def home():
    return render_template('home.html', title="WikiStore")

@app.route('/all') 
def all():
    return render_template('all.html', title="WikiStore")

@app.route('/<int:id>')
def page(id):
    return render_template(f'page.html', title=keyword[id], cont=open(f'data/mydata_{id}.html', 'r', encoding='utf-8').read())

@app.route('/search')
def search():
    k = request.args['q']
    return render_template('default.html', keyword=base.name(request.args['q']))

if __name__ == '__main__':
    app.run(port=5000, debug=True)