from flask import Flask, request, render_template, session, redirect, url_for
import base
# from search import search

def search(word):
    return base.search(word)

app = Flask(
    "dianlei",
    static_folder="./static",
    template_folder="./templates"
)

app.secret_key = b'_5#y2L"F4Q8z\n\xec]/'

def username():
    if 'username' in session: return session['username']
    else: return None

@app.route('/') 
def home():
    return render_template('home.html', title="Home", username=username())

@app.route('/all') 
def all():
    return render_template('all.html', title="All", keyword=base.all(), username=username())

@app.route('/<int:id>')
def page(id):
    title, cont = base.id(id)
    return render_template('page.html', id=id, title=title, cont=cont, username=username(), like=base.like(username(), id))

@app.route('/search')
def search_():
    word = request.args['q']
    return render_template('searchres.html', title="SearchResult", keyword=search(word), word=word, username=username())

@app.route('/graph') 
def graph():
    return render_template('graph.html', title="All", username=username())

@app.route('/favorite') 
def favorite():
    return render_template('all.html', title="Favorite", username=username(), keyword=base.choosefrom(username()))

@app.route('/login', methods=['GET', 'POST'])
def login():
    if 'username' in session:
        return render_template('login.html', username=username(), stat='logged');
    if request.method == 'POST':
        if base.check(request.form['username'], request.form['password']):
            session['username'] = request.form['username']  
            return redirect(url_for('home'))    
        else: return render_template('login.html', username=username(), stat='wrong')
    return render_template('login.html', username=username(), stat='login')

@app.route('/logout')
def logout():
    session.pop('username', None)
    return redirect(url_for('home'))

@app.route('/update')
def update():
    id, like = request.args['id'], request.args['like']
    if like == "1": base.addlike(username(), id)
    else: base.removelike(username(), id)
    return ''

if __name__ == '__main__':
    app.run(port=5000, debug=True)