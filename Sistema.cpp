/*
    Archivo: /home/deivi/Proyectos/2do/Sistema.cpp
    Sistema simple para gestionar libros en una pequeña biblioteca.
    Compilar: g++ -std=c++17 Sistema.cpp -o Sistema
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class Libro {
public:
        Libro() = default;
        Libro(std::string t, std::string a, std::string i)
                : titulo(std::move(t)), autor(std::move(a)), isbn(std::move(i)), disponible(true) {}

        const std::string& getTitulo() const { return titulo; }
        const std::string& getAutor() const { return autor; }
        const std::string& getISBN() const { return isbn; }
        bool isDisponible() const { return disponible; }
        void setDisponible(bool d) { disponible = d; }

        void mostrar() const {
                std::cout << "[" << isbn << "] " << titulo << " - " << autor
                                    << " (" << (disponible ? "Disponible" : "Prestado") << ")\n";
        }

private:
        std::string titulo;
        std::string autor;
        std::string isbn;
        bool disponible = true;
};

class Usuario {
public:
        Usuario(std::string id_, std::string nombre_) : id(std::move(id_)), nombre(std::move(nombre_)) {}
        virtual ~Usuario() = default;
        const std::string& getId() const { return id; }
        const std::string& getNombre() const { return nombre; }
        virtual int limitePrestamos() const = 0;

private:
        std::string id;
        std::string nombre;
};

class Estudiante : public Usuario {
public:
        Estudiante(std::string id, std::string nombre) : Usuario(std::move(id), std::move(nombre)) {}
        int limitePrestamos() const override { return 3; }
};

class Profesor : public Usuario {
public:
        Profesor(std::string id, std::string nombre) : Usuario(std::move(id), std::move(nombre)) {}
        int limitePrestamos() const override { return 10; }
};

class Biblioteca {
public:
        bool agregarLibro(const Libro& libro) {
                if (buscarIndicePorISBN(libro.getISBN()) != -1) {
                        std::cout << "El libro con ISBN " << libro.getISBN() << " ya existe.\n";
                        return false;
                }
                libros.push_back(libro);
                return true;
        }

        bool eliminarLibro(const std::string& isbn) {
                int idx = buscarIndicePorISBN(isbn);
                if (idx == -1) {
                        std::cout << "No se encontró libro con ISBN " << isbn << ".\n";
                        return false;
                }
                if (prestadoA.count(isbn)) {
                        std::cout << "No se puede eliminar. El libro está prestado a " << prestadoA[isbn] << ".\n";
                        return false;
                }
                libros.erase(libros.begin() + idx);
                std::cout << "Libro con ISBN " << isbn << " eliminado.\n";
                return true;
        }

        std::vector<Libro> buscarPorTitulo(const std::string& q) const {
                std::vector<Libro> res;
                for (const auto& l : libros) {
                        if (toLower(l.getTitulo()).find(toLower(q)) != std::string::npos) res.push_back(l);
                }
                return res;
        }

        std::vector<Libro> buscarPorAutor(const std::string& q) const {
                std::vector<Libro> res;
                for (const auto& l : libros) {
                        if (toLower(l.getAutor()).find(toLower(q)) != std::string::npos) res.push_back(l);
                }
                return res;
        }

        void mostrarDisponibles() const {
                std::cout << "Libros disponibles:\n";
                for (const auto& l : libros) {
                        if (l.isDisponible()) l.mostrar();
                }
        }

        bool prestarLibro(const std::string& isbn, const Usuario& usuario) {
                int idx = buscarIndicePorISBN(isbn);
                if (idx == -1) {
                        std::cout << "No existe libro con ISBN " << isbn << ".\n";
                        return false;
                }
                if (!libros[idx].isDisponible()) {
                        std::cout << "El libro ya está prestado.\n";
                        return false;
                }
                int actuales = prestamosPorUsuario[usuario.getId()].size();
                if (actuales >= usuario.limitePrestamos()) {
                        std::cout << usuario.getNombre() << " ha alcanzado el límite de préstamos (" << usuario.limitePrestamos() << ").\n";
                        return false;
                }
                libros[idx].setDisponible(false);
                prestadoA[isbn] = usuario.getId();
                prestamosPorUsuario[usuario.getId()].push_back(isbn);
                std::cout << "Libro " << isbn << " prestado a " << usuario.getNombre() << ".\n";
                return true;
        }

        bool devolverLibro(const std::string& isbn) {
                int idx = buscarIndicePorISBN(isbn);
                if (idx == -1) {
                        std::cout << "No existe libro con ISBN " << isbn << ".\n";
                        return false;
                }
                auto it = prestadoA.find(isbn);
                if (it == prestadoA.end()) {
                        std::cout << "El libro no está prestado.\n";
                        return false;
                }
                std::string userId = it->second;
                // marcar disponible
                libros[idx].setDisponible(true);
                // quitar de mapas
                prestadoA.erase(it);
                auto &v = prestamosPorUsuario[userId];
                v.erase(std::remove(v.begin(), v.end(), isbn), v.end());
                if (v.empty()) prestamosPorUsuario.erase(userId);
                std::cout << "Libro " << isbn << " devuelto por usuario " << userId << ".\n";
                return true;
        }

        void mostrarPrestamos() const {
                std::cout << "Préstamos actuales:\n";
                for (const auto& p : prestadoA) {
                        std::cout << "  ISBN " << p.first << " -> Usuario " << p.second << "\n";
                }
        }

private:
        std::vector<Libro> libros;
        std::unordered_map<std::string, std::string> prestadoA; // isbn -> userId
        std::unordered_map<std::string, std::vector<std::string>> prestamosPorUsuario; // userId -> isbns

        static std::string toLower(std::string s) {
                std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
                return s;
        }

        int buscarIndicePorISBN(const std::string& isbn) const {
                for (size_t i = 0; i < libros.size(); ++i) {
                        if (libros[i].getISBN() == isbn) return static_cast<int>(i);
                }
                return -1;
        }
};

int main() {
        Biblioteca bib;

        // Agregar libros
        bib.agregarLibro(Libro("C++ Primer", "Lippman", "ISBN-001"));
        bib.agregarLibro(Libro("Effective C++", "Meyers", "ISBN-002"));
        bib.agregarLibro(Libro("Clean Code", "Robert C. Martin", "ISBN-003"));
        bib.agregarLibro(Libro("Estructuras de Datos", "Goodrich", "ISBN-004"));

        bib.mostrarDisponibles();
        std::cout << "\n";

        // Crear usuarios
        Estudiante juan("u001", "Juan Pérez");
        Profesor ana("u002", "Ana López");

        // Prestar libros
        bib.prestarLibro("ISBN-001", juan);
        bib.prestarLibro("ISBN-002", juan);
        bib.prestarLibro("ISBN-003", juan);
        // Intento exceder límite del estudiante
        bib.prestarLibro("ISBN-004", juan);

        // Profesor pide un libro
        bib.prestarLibro("ISBN-004", ana);

        std::cout << "\n";
        bib.mostrarDisponibles();
        std::cout << "\n";
        bib.mostrarPrestamos();
        std::cout << "\n";

        // Buscar por autor
        auto encontrados = bib.buscarPorAutor("Meyers");
        std::cout << "Resultados búsqueda por autor 'Meyers':\n";
        for (const auto& l : encontrados) l.mostrar();
        std::cout << "\n";

        // Devolver libro y eliminar
        bib.devolverLibro("ISBN-002");
        bib.eliminarLibro("ISBN-002"); // ahora debe poder eliminar

        std::cout << "\nEstado final de la biblioteca:\n";
        bib.mostrarDisponibles();
        bib.mostrarPrestamos();

        return 0;
}