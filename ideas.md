Для лога сделать возможность задания длины строки.

Добавить возможность обрезать объекты.

Переделать рендеринг, чтобы рассеивание света зависело только от самой фигуры, а не от ее материала. Т.е. метод scatter был вынесен в класс объекта.
	Тогда все имеющиеся фигуры, которые поддаются полиморфизму, можно вынести в наследование от отдельного класса, у которого в функции рассеивания вызывается указатель на функцию материала.
		И там добавить настройку, чтобы можно было указывать для внутренней и внешней поверхности материалы.
			Внешняя - когда угол между нормалью и лучом камеры по модулю больше 90, иначе это внутренняя сторона.

Сделать устройство рендера, которое работает только с точечными источниками света и небом.

Для цвета выделить отдельный тип данных.
	Сделать так же рендеринг прозрачного цвета.

Переделать сцену, чтобы методы трансформации луча выполялись над каждым объектом в поиске пересечения сцены.
	Добавить трансформацию LookAt.

Сделать камеру в 360 градусов.

Для неявно заданных поверхностей добавить:
	Еще поверхности. 
		https://www.cs.williams.edu/~morgan/cs371-f14/reading/implicit.pdf 
		https://pdfs.semanticscholar.org/3e3a/0a92e9a8533b306ed6d75c7a08da700be8a7.pdf
	Методы объединения поверхностей.
		http://www.iquilezles.org/www/articles/smin/smin.htm

-------------------------------------------------------------------------------

Сделать сохранение файлов недорендеренных в raw формате, для последующего рендеринга.

Сделать класс, который будет отвечать за рендеринг этих файлов.
Сделать класс, который будет делать тоже самое для анимаций.
	Чтобы можно было отрендерить предварительно анимацию с несколькими семплами, затем снова пройтись по всем файлам и добавить еще семплов. Когда все закончится, преобразовать файлы анимации в картинки.

Когда будет готов модуль для окон сделать вывод 

Когда-нибудь сделать подповерхностное рассеивание и облака.
	https://www.solidangle.com/research/egsr2012_volume.pdf
	https://cseweb.ucsd.edu/~ravir/6998/papers/p165-hanrahan.pdf
	https://graphics.pixar.com/library/PhotonBeamDiffusion/paper.pdf
	https://www.semanticscholar.org/paper/Interactive-directional-subsurface-scattering-and-Corso-Frisvad/62b68e4535f405f6d54cf339fcd2d96d0cf349b0

-------------------------------------------------------------------------------

Эксперименты:
	Линзы.
		Методом объединения двух сфер и\или цилиндров.
			Протестировать разные радиусы.
			Протестировать разные коэфициенты преломления. (от -3 до 3)
	Зеркала.
		Зеркальная комната.
		Внутри зеркальной сферы.
		Сделать зеркало из сегмента сферы.
		Половины тора.
	Портальная сфера.
	Портальный полигон.
		Портальный куб.
		Освещение порталов.
		Сделать физику перемещения объектов в портале методом обрезания.